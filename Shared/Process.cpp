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
#include "Process.h"
#include <tuple>

namespace Filesystem = std::filesystem;

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

using Shared::Infrastructure::HandleWithNullForEmpty;
using Shared::Infrastructure::HandleWithInvalidForEmpty;

namespace Shared::Model {

    unique_ptr<Process> Process::Start(string_view const& filename, string_view const& arguments) {
        const auto absolutePath = Filesystem::absolute(filename).string();

        if (!Filesystem::exists(absolutePath) || !Filesystem::is_regular_file(absolutePath))
            throw std::invalid_argument("file not found");

        STARTUPINFOA startupInfo{};
        startupInfo.cb = sizeof(startupInfo);
        startupInfo.dwFlags = STARTF_USESTDHANDLES;
        PROCESS_INFORMATION processInformation{};

        unique_ptr<Process> process{};
        if (!CreateProcessAdapter(absolutePath, arguments, &startupInfo, &processInformation))
            return process;

        // make_unique won't work unless we do some trickery to make it a friend function
        return unique_ptr<Process>(new Process(processInformation)); 

    }

    vector<unique_ptr<Process>> Process::GetProcessesByName(string_view const& processName) {
        auto const entries = GetProcessEntries();
        vector<unique_ptr<Process>> filtered{};
        for (auto const& processEntry : entries) {
            if (auto const exeView = wstring_view(processEntry.szExeFile, wcslen(processEntry.szExeFile));
                string_equal(processName, exeView, true)) {
                filtered.emplace_back(new Process(processEntry.th32ProcessID));
            }
        }
        return filtered;
    }

    unsigned long Process::GetId() const noexcept {
        return processId;
    }

    bool Process::IsRunning() const noexcept {
        if (!static_cast<bool>(processHandle))
            return false;

        try {
            bool isRunning{};
            tie(isRunning, ignore) = GetRunningDetails(processHandle.Get());;
            return isRunning;

        } catch (std::exception const&) {
            return false;
        }
    }

    std::optional<unsigned long> Process::ExitCode() const noexcept {
        try {
            if (!static_cast<bool>(processHandle))
                return nullopt;

            bool isRunning{};
            unsigned long exitCode{};
            tie(isRunning, exitCode) = GetRunningDetails(processHandle.Get());

            return !isRunning
                ? optional<unsigned long>(exitCode)
                : nullopt;
        }
        catch (std::runtime_error const&) {
            return nullopt;
        }
    }
    void Process::WaitForExit() const noexcept {
        if (IsRunning())
            WaitForSingleObject(processHandle.Get(), INFINITE);
    }

    optional<Filesystem::path> Process::GetPathToRunningProcess(string_view const& processName) const noexcept {

        try {
            auto const process = GetProcessByName(processName);
            if (!process.has_value())
                return nullopt;

            auto const module = GetModuleByIdAndName(process.value().th32ProcessID, processName);
            if (!module.has_value())
                return nullopt;

            return optional(Filesystem::path(module.value().szExePath));
        } catch (std::exception const&) {
            return nullopt;
        }
    }

    Process::Process(unsigned long const processId)
        : processId(processId) {
        processHandle.Reset( OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId));
    }

    Process::Process(PROCESS_INFORMATION const& processInformation) {
        processHandle.Reset(processInformation.hProcess);
        processThreadHandle.Reset(processInformation.hThread);
        processId = processInformation.dwProcessId;
        processThreadId = processInformation.dwThreadId;
        processLaunched = true;
    }

    Process::Process(Process&& other) noexcept
        : processLaunched{other.processLaunched} 
        , processId{other.processId}
        , processThreadId{other.processThreadId} {

        processHandle = move(other.processHandle);
        processThreadHandle = move(other.processThreadHandle);

        other.processThreadId = 0UL;
        other.processId = 0UL;
        other.processLaunched = false;
    }
    Process& Process::operator=(Process&& other) noexcept {
        processHandle = move(other.processHandle);
        processThreadHandle = move(other.processThreadHandle);
        processId = other.processId;
        processThreadId = other.processThreadId;
        processLaunched = other.processLaunched;

        other.processThreadId = 0UL;
        other.processId = 0UL;
        other.processLaunched = false;
        return *this;
    }

    Process::~Process() {
        if (processLaunched && IsRunning())
            WaitForExit();
        processLaunched = false;
        processId = 0UL;
        processThreadId = 0UL;
    }

    bool Process::Equals(Process const& other) const noexcept { 
        return processId == other.processId &&
            processId == other.processThreadId &&
            processHandle.Get() == other.processHandle.Get() &&
            processThreadHandle.Get() == other.processThreadHandle.Get();
    }

    tuple<bool, unsigned long> Process::GetRunningDetails(HANDLE processHandle) {
        DWORD exitCode{};

        auto const getExitProcessSuccess = GetExitCodeProcess(processHandle, &exitCode);
        if (!getExitProcessSuccess)
            throw std::runtime_error(("GetExitCodeProcess failed with "s + to_string(GetLastError())).c_str());

        return exitCode == STILL_ACTIVE
            ? make_tuple(true, 0UL)
            : make_tuple(false, exitCode);
    }

    optional<PROCESSENTRY32> Process::GetProcessByName(std::string_view const& processName) noexcept {
        auto const entries = GetProcessEntries(); // if we were concerned with performance this would be a bad idea
        auto const matchingEntry = find_if(begin(entries), end(entries), 
            [&processName](auto const& entry) {
                auto const exeView = wstring_view(entry.szExeFile, wcslen(entry.szExeFile));
                return string_equal(processName, exeView, true);
            });

        return matchingEntry != entries.end()
            ? optional(*matchingEntry)
            : nullopt;
    }

    vector<PROCESSENTRY32> Process::GetProcessEntries() noexcept {
        HandleWithInvalidForEmpty const processSnapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD, 0));
        if (!static_cast<bool>(processSnapshot))
            return vector<PROCESSENTRY32>();

        PROCESSENTRY32 entry{};
        entry.dwSize = sizeof(PROCESSENTRY32);
        if (!Process32First(processSnapshot.Get(), &entry))
            return vector<PROCESSENTRY32>();

        vector<PROCESSENTRY32> processes;
        do {
            processes.push_back(entry);

        } while (Process32Next(processSnapshot.Get(), &entry));

        return processes;
    }

    optional<MODULEENTRY32> Process::GetModuleByIdAndName(unsigned long const processId, string_view const& processName) noexcept {
        auto const modules = GetModuleEntries(processId);
        auto const matchingModule =  find_if(begin(modules), end(modules), 
            [processName](auto const& module) {
                wstring_view const moduleName(module.szModule, wcslen(module.szModule));
                return string_equal(processName, moduleName, true);
            });

        return matchingModule != modules.end()
            ? optional(*matchingModule)
            : nullopt;
    }

    std::vector<MODULEENTRY32> Process::GetModuleEntries(unsigned long const processId) noexcept {
        HandleWithInvalidForEmpty const snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId));
        MODULEENTRY32 entry{};
        entry.dwSize = sizeof(MODULEENTRY32);

        if (!Module32First(snapshot.Get(), &entry))
            return vector<MODULEENTRY32>();

        vector<MODULEENTRY32> modules;
        do {
            modules.push_back(entry);

        } while (Module32Next(snapshot.Get(), &entry));

        return modules;
    }

    bool Process::CreateProcessAdapter(string_view const& filename, string_view const& arguments, STARTUPINFOA * const startupInfo, PROCESS_INFORMATION * const processInfo) {
        auto const maxPath = 256UL;
        auto const commandLine = make_unique<char[]>(32768); // max size of command line which cannot be readonly
        auto const filenameLength = std::min<size_t>(filename.size(), maxPath);

        filename.copy(commandLine.get(), filenameLength);
        commandLine[filenameLength] = ' ';
        arguments.copy(&commandLine.get()[filenameLength+1], std::min<size_t>(arguments.size(), 32768L - maxPath - 1UL));

        auto *const commandLineString = commandLine.get();

        return CreateProcessA(nullptr, commandLineString, nullptr, nullptr, TRUE, CREATE_NO_WINDOW, 
            nullptr, nullptr, startupInfo, processInfo) == TRUE;
    }

    bool operator==(Process const& leftHandSide, Process const& rightHandSide) {
        return &leftHandSide == &rightHandSide || leftHandSide.Equals(rightHandSide);
    }

}