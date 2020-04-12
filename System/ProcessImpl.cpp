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
#include <tuple>

namespace Filesystem = std::filesystem;

using std::string;
using std::optional;
using std::nullopt;
using std::make_unique;
using std::min;
using std::tuple;

namespace System::Implementation
{
    ProcessImpl::ProcessImpl(string const& filename, string const& arguments)
    {
        if (!Filesystem::exists(filename) || !Filesystem::is_regular_file(filename))
            throw std::invalid_argument("file not found");

        if (!CreateProcessAdapter(filename, arguments, &_startupInfo, &_processInformation))
            throw std::bad_exception();
    }

    optional<DWORD> ProcessImpl::GetId() const noexcept
    {
        return _processInformation.dwProcessId != 0L
            ? optional<DWORD>(_processInformation.dwProcessId)
            : nullopt;
    }
    bool ProcessImpl::IsRunning() const noexcept
    {
        const auto details = GetRunningDetails(_processInformation.hProcess);
        return std::get<bool>(details);
    }
    std::optional<DWORD> ProcessImpl::ExitCode() const noexcept
    {
        const auto details = GetRunningDetails(_processInformation.hProcess);
        return std::get<bool>(details)
            ? optional<DWORD>(std::get<DWORD>(details))
            : nullopt;
    }
    void ProcessImpl::WaitForExit() const
    {
        if (!IsRunning())
            return;
        WaitForSingleObject(_processInformation.hProcess, INFINITE);
    }

    std::tuple<bool, DWORD> ProcessImpl::GetRunningDetails(HANDLE processHandle)
    {
        DWORD exitCode{};
        if (GetExitCodeProcess(processHandle, &exitCode) || GetLastError() != STILL_ACTIVE)
            return tuple(false, exitCode);

        return WaitForSingleObject(processHandle, 0) == WAIT_TIMEOUT
            ? tuple(true, exitCode)
            : tuple(false, exitCode);
    }
    bool ProcessImpl::CreateProcessAdapter(string const& filename, string const& arguments, STARTUPINFOA * const startupInfo, PROCESS_INFORMATION * const processInfo)
    {
        const auto commandLine = make_unique<char[]>(32768); // max size of command line which cannot be readonly
        arguments.copy(commandLine.get(), std::min<size_t>(arguments.size(), 32768L));

        return CreateProcessA(filename.c_str(), commandLine.get(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, 
            nullptr, nullptr, startupInfo, processInfo) == TRUE;
    }

}
