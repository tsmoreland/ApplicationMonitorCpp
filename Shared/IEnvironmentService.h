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

#include <optional>
#include <memory>
#include <string_view>
#include <vector>
#include "IProcess.h"

namespace Shared::Services
{
    struct IEnvironmentService
    {
        [[nodiscard]] virtual std::optional<std::unique_ptr<Shared::Model::IProcess>> StartProcess(std::string_view const& filename, std::string_view const& arguments) const noexcept = 0;
        [[nodiscard]] virtual std::vector<std::unique_ptr<Shared::Model::IProcess>> GetProcessesByName(std::string_view const& processName) const noexcept = 0;

        [[nodiscard]] virtual std::optional<std::string> GetVariable(std::string const& key) const noexcept = 0;
        [[nodiscard]] virtual bool SetVariable(std::string const& key, std::string const& value) const noexcept = 0;

        [[nodiscard]] virtual std::vector<std::filesystem::path> GetFilesFromDirectory(std::filesystem::path const& folder, std::wstring_view const filter) const noexcept = 0;

        IEnvironmentService() = default;
        IEnvironmentService(IEnvironmentService const&) = default;
        IEnvironmentService& operator=(IEnvironmentService const&) = default;
        IEnvironmentService(IEnvironmentService&&) noexcept = default;
        IEnvironmentService& operator=(IEnvironmentService&& other) noexcept = default;
        virtual ~IEnvironmentService() = default;

    };
}

