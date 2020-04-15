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

#pragma once

namespace Win32::Implementation
{
    class ProcessImpl final
    {
    public:
        ProcessImpl(std::string const& filename, std::string const& arguments);
        ProcessImpl(const ProcessImpl&) = delete;
        ProcessImpl& operator=(const ProcessImpl&) = delete;
        ProcessImpl(ProcessImpl&& other) noexcept;
        ProcessImpl& operator=(ProcessImpl&& other) noexcept;
        ~ProcessImpl() = default;

        [[nodiscard]] std::optional<DWORD> GetId() const noexcept;
        [[nodiscard]] bool IsRunning() const noexcept;
        [[nodiscard]] std::optional<DWORD> ExitCode() const noexcept;
        void WaitForExit() const;

    private:
        DWORD _processId;
        DWORD _processThreadId;
        Shared::NullHandle _processHandle;
        Shared::NullHandle _processThreadHandle;

        static bool CreateProcessAdapter(std::string const& filename, std::string const& arguments, STARTUPINFOA * const startupInfo, PROCESS_INFORMATION * const processInfo);
        static std::tuple<bool, DWORD> GetRunningDetails(HANDLE processHandle);
        void LoadProcessInformation(const PROCESS_INFORMATION& processInformation);
    };

}

