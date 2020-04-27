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
#include "Settings.h"
#include "IFileService.h"
#include <CommandResult.h>

namespace DebugSymbolManager::Model {

    class NtSymbolPath final {
    public:
        [[nodiscard]] std::optional<std::string> GetSymbolPath() const noexcept;

        [[nodiscard]] std::optional<std::string> const& GetLocalCache() const noexcept;
        [[nodiscard]] Shared::Model::CommandResult SetLocalCache(std::string const& value) noexcept;

        [[nodiscard]] std::string const& GetSymbolServer() const noexcept;
        void SetSymbolServer(std::string server) noexcept;

        void AddDirectory(std::string const& directory) noexcept;
        void RemoveDirectory(std::string const& directory) noexcept;

        [[nodiscard]] bool IsModified() const noexcept;
        void ResetModified() noexcept;

        explicit NtSymbolPath(Shared::Service::IFileService const& fileService);
        NtSymbolPath(NtSymbolPath const&) = default;
        NtSymbolPath(NtSymbolPath&&) noexcept = default;
        ~NtSymbolPath() = default;

        NtSymbolPath& operator=(NtSymbolPath const&) = delete;
        NtSymbolPath& operator=(NtSymbolPath&&) = delete;

    private:
        std::string lastSavedState{};
        bool isModified{false};
        std::optional<std::string> localCache;
        std::string symbolServer;
        Shared::Service::IFileService const& fileService;
        std::vector<std::filesystem::path> additionalPaths;
        std::string baseSymbolPath;

        void UpdateIsModified() noexcept;
    };
}
