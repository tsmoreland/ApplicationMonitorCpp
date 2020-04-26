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
#include <vector>
#include "Common.h"
#include "Settings.h"
#include <IEnvironmentService.h>

namespace DebugSymbolManager::Model {

    class NtSymbolPath final {
    public:
        [[nodiscard]] std::optional<std::string> const& GetLocalCache() const noexcept;
        bool SetLocalCache(std::string const& value) noexcept;
        [[nodiscard]] std::optional<std::string> GetSymbolPath() const noexcept;

        void AddDirectory(std::string const& directory) noexcept;
        void RemoveDirectory(std::string const& directory) noexcept;

        explicit NtSymbolPath(Shared::Services::IEnvironmentService const& environmentService, Settings const& settings);
        NtSymbolPath(NtSymbolPath const&) = default;
        NtSymbolPath(NtSymbolPath&&) noexcept = default;
        ~NtSymbolPath() = default;

        NtSymbolPath& operator=(NtSymbolPath const&) = delete;
        NtSymbolPath& operator=(NtSymbolPath&&) = delete;

    private:
        std::optional<std::string> localCache;
        Shared::Services::IEnvironmentService const& environmentService;
        std::vector<std::filesystem::path> additionalPaths;
        std::string baseSymbolPath;
    };
}
