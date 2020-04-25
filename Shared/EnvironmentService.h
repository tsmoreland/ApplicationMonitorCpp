//
// Copyright � 2020 Terry Moreland
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

#include "IEnvironmentService.h"

namespace Shared::Services
{
    class EnvironmentService final : public IEnvironmentService
    {
    public:
        [[nodiscard]] SHARED_DLL virtual std::optional<std::unique_ptr<Shared::Model::IProcess>> StartProcess(std::string_view const& filename, std::string_view const& arguments) const noexcept override;
        [[nodiscard]] SHARED_DLL virtual std::vector<std::unique_ptr<Shared::Model::IProcess>> GetProcessesByName(std::string_view const& processName) const noexcept override;

        [[nodiscard]] SHARED_DLL virtual std::optional<std::string> GetVariable(std::string const& key) const noexcept override;
        [[nodiscard]] SHARED_DLL virtual bool SetVariable(std::string const& key, std::string const& value) const noexcept override;

        [[nodiscard]] SHARED_DLL virtual std::vector<std::filesystem::path> GetFilesFromDirectory(std::filesystem::path const& folder, std::wregex const& filter) const noexcept override;
        [[nodiscard]] SHARED_DLL std::optional<std::filesystem::path> GetPathToRunningProcess(std::string_view const& processName) const noexcept override;

        SHARED_DLL EnvironmentService() = default;
        SHARED_DLL EnvironmentService(const EnvironmentService&) = default;
        SHARED_DLL EnvironmentService(EnvironmentService&&) noexcept = default;
        SHARED_DLL EnvironmentService& operator=(const EnvironmentService&) = default;
        SHARED_DLL EnvironmentService& operator=(EnvironmentService&&) noexcept = default;
        SHARED_DLL ~EnvironmentService() = default;

    };

}