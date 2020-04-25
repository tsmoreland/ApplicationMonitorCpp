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

#include "Export.h"

namespace Shared::Model
{
    struct IProcess
    {
        [[nodiscard]] SHARED_DLL virtual unsigned long GetId() const noexcept = 0;
        [[nodiscard]] SHARED_DLL virtual bool IsRunning() const noexcept = 0;
        [[nodiscard]] SHARED_DLL virtual std::optional<unsigned long> ExitCode() const noexcept = 0;
        SHARED_DLL virtual void WaitForExit() const noexcept = 0;
        [[nodiscard]] SHARED_DLL virtual std::optional<std::filesystem::path> GetPathToRunningProcess(std::string_view const& processName) const noexcept = 0;

        SHARED_DLL IProcess() = default;
        IProcess(const IProcess&) = delete;
        IProcess& operator=(const IProcess&) = delete;
        SHARED_DLL IProcess(IProcess&&) = default;
        SHARED_DLL IProcess& operator=(IProcess&&) = default;
        SHARED_DLL virtual ~IProcess() = default;

    };
}
