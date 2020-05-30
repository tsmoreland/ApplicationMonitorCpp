//
// Copyright © 2020 Terry Moreland
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 

#include "pch.h"
#include "process_impl.h"
#include <tuple>

using std::find_if;
using std::ignore;
using std::make_tuple;
using std::make_unique;
using std::min;
using std::move;
using std::nullopt;
using std::optional;
using std::string;
using std::string_view;
using std::tie;
using std::to_string;
using std::tuple;
using std::unique_ptr;
using std::vector;
using std::wstring_view;

using extension::string_equal;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator""s;
#pragma warning(pop)

using shared::infrastructure::null_handle;
using shared::infrastructure::invalid_handle;
using shared::model::unique_process;

namespace shared::model
{

unique_process process_impl::start(string_view const& filename, string_view const& arguments)
{
    const auto absolutePath = std::filesystem::absolute(filename).string();

    if (!std::filesystem::exists(absolutePath) || !std::filesystem::is_regular_file(absolutePath))
        throw std::invalid_argument("file not found");

    STARTUPINFOA startupInfo{};
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    PROCESS_INFORMATION process_information{};

    unique_process process{};
    if (!create_process_adapter(absolutePath, arguments, &startupInfo, &process_information))
        return process;

    // make_unique won't work unless we do some trickery to make it a friend function
    return unique_process(new process_impl(process_information));
}

vector<unique_process> process_impl::get_processes_by_name(string_view const& process_name)
{
    auto const entries = get_process_entries();
    vector<unique_process> filtered{};
    for (auto const& processEntry : entries) {
        if (auto const exeView = wstring_view(processEntry.szExeFile, wcslen(processEntry.szExeFile));
            string_equal(process_name, exeView, true)) {
            filtered.emplace_back(new process_impl(processEntry.th32ProcessID));
        }
    }
    return filtered;
}

unsigned long process_impl::get_id() const noexcept
{
    return m_process_id;
}

bool process_impl::is_running() const noexcept
{
    if (!static_cast<bool>(m_process_handle))
        return false;

    try {
        bool isRunning{};
        tie(isRunning, ignore) = get_running_details(m_process_handle.get());;
        return isRunning;

    } catch (std::exception const&) {
        return false;
    }
}

std::optional<unsigned long> process_impl::exit_code() const noexcept
{
    try {
        if (!static_cast<bool>(m_process_handle))
            return nullopt;

        bool isRunning{};
        unsigned long exitCode{};
        tie(isRunning, exitCode) = get_running_details(m_process_handle.get());

        return !isRunning
            ? optional<unsigned long>(exitCode)
            : nullopt;
    }
    catch (std::runtime_error const&) {
        return nullopt;
    }
}
void process_impl::wait_for_exit() const noexcept {
    if (is_running())
        WaitForSingleObject(m_process_handle.get(), INFINITE);
}

optional<std::filesystem::path> process_impl::get_path_to_running_process(string_view const& process_name) const noexcept
{
    try {
        auto const process = get_process_by_name(process_name);
        if (!process.has_value())
            return nullopt;

        auto const module = get_module_by_id_and_name(process.value().th32ProcessID, process_name);
        if (!module.has_value())
            return nullopt;

        return optional(std::filesystem::path(module.value().szExePath));
    } catch (std::exception const&) {
        return nullopt;
    }
}

process_impl::process_impl(unsigned long const process_id)
    : m_process_id(process_id) {
    m_process_handle.Reset( OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id));
}

process_impl::process_impl(PROCESS_INFORMATION const& process_information)
{
    m_process_handle.Reset(process_information.hProcess);
    m_process_thread_handle.Reset(process_information.hThread);
    m_process_id = process_information.dwProcessId;
    m_process_thread_id = process_information.dwThreadId;
    m_process_launched = true;
}

process_impl::process_impl(process_impl&& other) noexcept
    : m_process_launched{other.m_process_launched} 
    , m_process_id{other.m_process_id}
    , m_process_thread_id{other.m_process_thread_id}
{

    swap(m_process_handle, other.m_process_handle);
    swap(m_process_thread_handle, other.m_process_thread_handle);

    other.m_process_thread_id = 0UL;
    other.m_process_id = 0UL;
    other.m_process_launched = false;
}
process_impl& process_impl::operator=(process_impl&& other) noexcept
{
    swap(m_process_handle, other.m_process_handle);
    swap(m_process_thread_handle, other.m_process_thread_handle);
    m_process_id = other.m_process_id;
    m_process_thread_id = other.m_process_thread_id;
    m_process_launched = other.m_process_launched;

    other.m_process_thread_id = 0UL;
    other.m_process_id = 0UL;
    other.m_process_launched = false;
    return *this;
}

process_impl::~process_impl()
{
    if (m_process_launched && is_running())
        wait_for_exit();
    m_process_launched = false;
    m_process_id = 0UL;
    m_process_thread_id = 0UL;
}

bool process_impl::equals(process_impl const& other) const noexcept
{
    return m_process_id == other.m_process_id &&
        m_process_thread_id == other.m_process_thread_id &&
        m_process_handle.get() == other.m_process_handle.get() &&
        m_process_thread_handle.get() == other.m_process_thread_handle.get();
}

tuple<bool, unsigned long> process_impl::get_running_details(HANDLE process_handle)
{
    DWORD exit_code{};
    auto const getExitProcessSuccess = GetExitCodeProcess(process_handle, &exit_code);
    if (!getExitProcessSuccess)
        throw std::runtime_error(("GetExitCodeProcess failed with "s + to_string(GetLastError())).c_str());

    return exit_code == STILL_ACTIVE
        ? make_tuple(true, 0UL)
        : make_tuple(false, exit_code);
}

optional<PROCESSENTRY32> process_impl::get_process_by_name(std::string_view const& process_name) 
{
    auto const entries = get_process_entries(); // if we were concerned with performance this would be a bad idea
    auto const matchingEntry = find_if(begin(entries), end(entries), 
        [&process_name](auto const& entry) {
            auto const exeView = wstring_view(entry.szExeFile, wcslen(entry.szExeFile));
            return string_equal(process_name, exeView, true);
        });

    return matchingEntry != entries.end()
        ? optional(*matchingEntry)
        : nullopt;
}

vector<PROCESSENTRY32> process_impl::get_process_entries() 
{
    invalid_handle const processSnapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD, 0));
    if (!static_cast<bool>(processSnapshot))
        return vector<PROCESSENTRY32>();

    PROCESSENTRY32 entry{};
    entry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(processSnapshot.get(), &entry))
        return vector<PROCESSENTRY32>();

    vector<PROCESSENTRY32> processes;
    do {
        processes.push_back(entry);

    } while (Process32Next(processSnapshot.get(), &entry));

    return processes;
}

optional<MODULEENTRY32> process_impl::get_module_by_id_and_name(unsigned long const process_id, string_view const& process_name) 
{
    auto const modules = get_module_entries(process_id);
    auto const matchingModule =  find_if(begin(modules), end(modules), 
        [process_name](auto const& module) {
            wstring_view const moduleName(module.szModule, wcslen(module.szModule));
            return string_equal(process_name, moduleName, true);
        });

    return matchingModule != modules.end()
        ? optional(*matchingModule)
        : nullopt;
}

std::vector<MODULEENTRY32> process_impl::get_module_entries(unsigned long const process_id) 
{
    invalid_handle const snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id));
    MODULEENTRY32 entry{};
    entry.dwSize = sizeof(MODULEENTRY32);

    if (!Module32First(snapshot.get(), &entry))
        return vector<MODULEENTRY32>();

    vector<MODULEENTRY32> modules;
    do {
        modules.push_back(entry);

    } while (Module32Next(snapshot.get(), &entry));

    return modules;
}

bool process_impl::create_process_adapter(string_view const& filename, string_view const& arguments, STARTUPINFOA * const startup_info, PROCESS_INFORMATION * const process_info)
{
    auto const maxPath = 256UL;
    auto const commandLine = make_unique<char[]>(32768); // max size of command line which cannot be readonly
    auto const filenameLength = std::min<size_t>(filename.size(), maxPath);

    filename.copy(commandLine.get(), filenameLength);
    commandLine[filenameLength] = ' ';
    arguments.copy(&commandLine.get()[filenameLength+1], std::min<size_t>(arguments.size(), 32768L - maxPath - 1UL));

    auto *const commandLineString = commandLine.get();

    return CreateProcessA(nullptr, commandLineString, nullptr, nullptr, TRUE, CREATE_NO_WINDOW, 
        nullptr, nullptr, startup_info, process_info) == TRUE;
}

bool operator==(process_impl const& left_hand_side, process_impl const& right_hand_side)
{
    return &left_hand_side == &right_hand_side || left_hand_side.equals(right_hand_side);
}

}