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
#include <tlhelp32.h>

namespace Filesystem = std::filesystem;

using std::make_unique;
using std::min;
using std::move;
using std::nullopt;
using std::optional;
using std::string;
using std::string_equal;
using std::string_view;
using std::tuple;
using std::unique_ptr;
using std::vector;
using std::wstring_view;

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

        auto& processes = processIterable.value();
        for (auto& process : processes)
        {
            if (!process.has_value())
                continue;
            
            if (auto const exeView = wstring_view(process.value().szExeFile, wcslen(process.value().szExeFile));
                string_equal(processName, exeView, true))
            {
            }
        }

        return vector<unique_ptr<ProcessImpl>>();
    }

    ProcessImpl::ProcessImpl()
        : _processId(0UL)
        , _processThreadId(0UL)
    {
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
    bool ProcessImpl::IsRunning() const noexcept
    {
        const auto details = GetRunningDetails(_processHandle.Get());
        return std::get<bool>(details);
    }
    std::optional<unsigned long> ProcessImpl::ExitCode() const noexcept
    {
        const auto details = GetRunningDetails(_processHandle.Get());
        return std::get<bool>(details)
            ? optional<unsigned long>(std::get<DWORD>(details))
            : nullopt;
    }
    void ProcessImpl::WaitForExit() const noexcept
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
        if (GetExitCodeProcess(processHandle, &exitCode) || GetLastError() != STILL_ACTIVE)
            return tuple(false, exitCode);

        return WaitForSingleObject(processHandle, 0) == WAIT_TIMEOUT
            ? tuple(true, exitCode)
            : tuple(false, exitCode);
    }
    bool ProcessImpl::CreateProcessAdapter(string const& filename, string_view const& arguments, STARTUPINFOA * const startupInfo, PROCESS_INFORMATION * const processInfo)
    {
        const auto commandLine = make_unique<char[]>(32768); // max size of command line which cannot be readonly
        arguments.copy(commandLine.get(), std::min<size_t>(arguments.size(), 32768L));

        return CreateProcessA(filename.c_str(), commandLine.get(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, 
            nullptr, nullptr, startupInfo, processInfo) == TRUE;
    }
    bool operator==(ProcessImpl const& leftHandSide, ProcessImpl const& rightHandSide)
    {
        return &leftHandSide == &rightHandSide ||
            &leftHandSide != nullptr && leftHandSide.Equals(rightHandSide);
    }

}
