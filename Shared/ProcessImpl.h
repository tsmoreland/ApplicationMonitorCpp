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
#include <TlHelp32.h>

namespace Shared::Infrastructure
{
    class ProcessImpl final
    {
    public:
        static std::unique_ptr<ProcessImpl> Start(std::string_view const& filename, std::string_view const& arguments);
        static std::vector<std::unique_ptr<ProcessImpl>> GetProcessesByName(std::string_view const& processName);

        [[nodiscard]] unsigned long GetId() const noexcept;
        [[nodiscard]] bool IsRunning() const;
        [[nodiscard]] std::optional<unsigned long> ExitCode() const noexcept;
        void WaitForExit() const; 
        static [[nodiscard]] std::optional<std::filesystem::path> GetPathToRunningProcess(std::string_view const& processName);

        ProcessImpl() = default;
        explicit ProcessImpl(unsigned long const processId);
        ProcessImpl(const ProcessImpl&) = delete;
        ProcessImpl& operator=(const ProcessImpl&) = delete;
        ProcessImpl(ProcessImpl&& other) noexcept;
        ProcessImpl& operator=(ProcessImpl&& other) noexcept;
        ~ProcessImpl();

        [[nodiscard]] bool Equals(ProcessImpl const& other) const noexcept;
    private:
        unsigned long _processId{};
        unsigned long _processThreadId{};
        HandleWithNullForEmpty _processHandle{};
        HandleWithNullForEmpty _processThreadHandle{};

        explicit ProcessImpl(PROCESS_INFORMATION const& processInformation);
        static bool CreateProcessAdapter(std::string_view const& filename, std::string_view const& arguments, STARTUPINFOA * const startupInfo, PROCESS_INFORMATION * const processInfo);
        static std::tuple<bool, unsigned long> GetRunningDetails(HANDLE processHandle);

        static std::optional<PROCESSENTRY32> GetProcessByName(std::string_view const& processName) noexcept;
        static std::vector<PROCESSENTRY32> GetProcessEntries() noexcept;
        static std::optional<MODULEENTRY32> GetModuleByIdAndName(unsigned long const processId, std::string_view const& processName) noexcept;
        static std::vector<MODULEENTRY32> GetModuleEntries(unsigned long const processId) noexcept;
    };

    bool operator==(ProcessImpl const& leftHandSide, ProcessImpl const& rightHandSide);

}

