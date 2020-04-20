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
#include "ProcessImpl.h"
#include "ProcessIterable.h"
#include <tuple>

namespace Filesystem = std::filesystem;

using std::make_unique;
using std::min;
using std::move;
using std::nullopt;
using std::optional;
using std::string;
using std::string_equal;
using std::string_view;
using std::to_string;
using std::tuple;
using std::unique_ptr;
using std::vector;
using std::wstring_view;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator""s;
#pragma warning(pop)

using boolinq::from;
using Shared::Infrastructure::HandleWithNullForEmpty;
using Shared::Infrastructure::HandleWithInvalidForEmpty;

namespace Shared::Infrastructure
{
    unique_ptr<ProcessImpl> ProcessImpl::Start(string_view const& filename, string_view const& arguments)
    {
        const auto absolutePath = Filesystem::absolute(filename).string();

        if (!Filesystem::exists(absolutePath) || !Filesystem::is_regular_file(absolutePath))
            throw std::invalid_argument("file not found");

        STARTUPINFOA startupInfo{};
        startupInfo.cb = sizeof(startupInfo);
        startupInfo.dwFlags = STARTF_USESTDHANDLES;
        //startupInfo.wShowWindow = SW_HIDE;
        PROCESS_INFORMATION processInformation{};

        unique_ptr<ProcessImpl> process{};
        if (!CreateProcessAdapter(absolutePath, arguments, &startupInfo, &processInformation))
            return process;

        // make_unique won't work unless we do some trickery to make it a friend function
        return unique_ptr<ProcessImpl>(new ProcessImpl(processInformation)); 

    }
    vector<unique_ptr<ProcessImpl>> ProcessImpl::GetProcessesByName(string_view const& processName)
    {
        const auto processIterable = ProcessIterable::GetProcesses();
        if (!processIterable.has_value())
            return vector<unique_ptr<ProcessImpl>>();

        auto const& processes = processIterable.value();
        vector<unique_ptr<ProcessImpl>> filtered{};
        for (auto& process : processes)
        {
            if (!process.has_value())
                continue;
            
            auto const procName = process.value().szExeFile;
            std::wcout << procName << std::endl;

            if (auto const exeView = wstring_view(process.value().szExeFile, wcslen(process.value().szExeFile));
                string_equal(processName, exeView, true))
            {
                filtered.emplace_back(new ProcessImpl(process.value().th32ProcessID));
            }
        }

        return filtered;
    }

    ProcessImpl::ProcessImpl(unsigned long const processId)
        : _processId(processId)
        , _processThreadId(0UL)
    {
        _processHandle.Reset( OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId));
    }
    ProcessImpl::ProcessImpl(PROCESS_INFORMATION const& processInformation)
    {
        _processHandle.Reset(processInformation.hProcess);
        _processThreadHandle.Reset(processInformation.hThread);
        _processId = processInformation.dwProcessId;
        _processThreadId = processInformation.dwThreadId;
    }
    ProcessImpl::ProcessImpl(ProcessImpl&& other) noexcept
        : _processId{other._processId}
        , _processThreadId{other._processThreadId}
    {
        _processHandle = move(other._processHandle);
        _processThreadHandle = move(other._processThreadHandle);

        other._processThreadId = 0UL;
        other._processId = 0UL;
    }
    ProcessImpl& ProcessImpl::operator=(ProcessImpl&& other) noexcept
    {
        _processHandle = move(other._processHandle);
        _processThreadHandle = move(other._processThreadHandle);
        _processId = other._processId;
        _processThreadId = other._processThreadId;

        other._processThreadId = 0UL;
        other._processId = 0UL;
        return *this;
    }

    unsigned long ProcessImpl::GetId() const noexcept
    {
        return _processId;
    }
    bool ProcessImpl::IsRunning() const 
    {
        const auto details = GetRunningDetails(_processHandle.Get());
        return std::get<bool>(details);
    }
    std::optional<unsigned long> ProcessImpl::ExitCode() const noexcept
    {
        try
        {
            const auto details = GetRunningDetails(_processHandle.Get());
            return std::get<bool>(details)
                ? optional<unsigned long>(std::get<DWORD>(details))
                : nullopt;
        }
        catch (std::runtime_error&)
        {
            return nullopt;
        }
    }
    void ProcessImpl::WaitForExit() const 
    {
        if (IsRunning())
            WaitForSingleObject(_processHandle.Get(), INFINITE);
    }

    bool ProcessImpl::Equals(ProcessImpl const& other) const noexcept
    { 
        if (&other == nullptr)
            return false;

        return _processId == other._processId &&
            _processId == other._processThreadId &&
            _processHandle.Get() == other._processHandle.Get() &&
            _processThreadHandle.Get() == other._processThreadHandle.Get();
    }
    tuple<bool, unsigned long> ProcessImpl::GetRunningDetails(HANDLE processHandle)
    {
        DWORD exitCode{};

        auto const getExitProcessSuccess = GetExitCodeProcess(processHandle, &exitCode);
        if (!getExitProcessSuccess)
            throw std::runtime_error(("GetExitCodeProcess failed with "s + to_string(GetLastError())).c_str());

        return exitCode == STILL_ACTIVE
            ? tuple(true, 0UL)
            : tuple(false, exitCode);
    }
    bool ProcessImpl::CreateProcessAdapter(string_view const& filename, string_view const& arguments, STARTUPINFOA * const startupInfo, PROCESS_INFORMATION * const processInfo)
    {
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
    bool operator==(ProcessImpl const& leftHandSide, ProcessImpl const& rightHandSide)
    {
        return &leftHandSide == &rightHandSide ||
            &leftHandSide != nullptr && leftHandSide.Equals(rightHandSide);
    }

}
