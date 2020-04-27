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
#include "IProcess.h"

namespace Shared::Model {

    class Process final : public IProcess {
    public:
        static std::unique_ptr<Process> Start(std::string_view const& filename, std::string_view const& arguments);
        static std::vector<std::unique_ptr<Process>> GetProcessesByName(std::string_view const& processName);

        [[nodiscard]] unsigned long GetId() const noexcept override;
        [[nodiscard]] bool IsRunning() const noexcept override;
        [[nodiscard]] std::optional<unsigned long> ExitCode() const noexcept override;
        void WaitForExit() const noexcept override; 
        [[nodiscard]] std::optional<std::filesystem::path> GetPathToRunningProcess(std::string_view const& processName) const noexcept override;

        Process() = default;
        explicit Process(unsigned long const processId);
        Process(const Process&) = delete;
        Process& operator=(const Process&) = delete;
        Process(Process&& other) noexcept;
        Process& operator=(Process&& other) noexcept;
        ~Process();

        [[nodiscard]] bool Equals(Process const& other) const noexcept;
    private:
        bool processLaunched{};
        unsigned long processId{};
        unsigned long processThreadId{};
        Shared::Infrastructure::HandleWithNullForEmpty processHandle{};
        Shared::Infrastructure::HandleWithNullForEmpty processThreadHandle{};

        explicit Process(PROCESS_INFORMATION const& processInformation);
        static bool CreateProcessAdapter(std::string_view const& filename, std::string_view const& arguments, STARTUPINFOA * const startupInfo, PROCESS_INFORMATION * const processInfo);
        static std::tuple<bool, unsigned long> GetRunningDetails(HANDLE processHandle);

        static std::optional<PROCESSENTRY32> GetProcessByName(std::string_view const& processName) noexcept;
        static std::vector<PROCESSENTRY32> GetProcessEntries() noexcept;
        static std::optional<MODULEENTRY32> GetModuleByIdAndName(unsigned long const processId, std::string_view const& processName) noexcept;
        static std::vector<MODULEENTRY32> GetModuleEntries(unsigned long const processId) noexcept;
    };

    bool operator==(Process const& leftHandSide, Process const& rightHandSide);

}

