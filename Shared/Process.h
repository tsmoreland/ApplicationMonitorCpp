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

#include <memory>
#include <optional>
#include <string>
#include "IProcess.h"

namespace Shared::Infrastructure
{
    class ProcessImpl;
}

namespace Shared::Domain
{
    class Process final : public IProcess
    {
    public:
        static std::unique_ptr<IProcess> Start(std::string_view const& filename, std::string_view const& arguments);
        static std::vector<std::unique_ptr<IProcess>> GetProcessesByName(std::string_view const& processName);

        [[nodiscard]] unsigned long GetId() const noexcept override;
        [[nodiscard]] bool IsRunning() const noexcept override;
        [[nodiscard]] std::optional<unsigned long> ExitCode() const noexcept override;
        void WaitForExit() const noexcept override;

        ~Process() override;
        Process(const Process&) = delete;
        Process& operator=(const Process&) = delete;
        Process(Process&&) noexcept = default;
        Process& operator=(Process&&) noexcept;

    private:
        std::unique_ptr<Shared::Infrastructure::ProcessImpl> _pImpl{};
        explicit Process(Shared::Infrastructure::ProcessImpl* pImpl);
    };

};
