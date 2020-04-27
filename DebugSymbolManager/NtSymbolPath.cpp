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
#include "NtSymbolPath.h"
#include <sstream>

using std::filesystem::path;
using std::find;
using std::move;
using std::nullopt;
using std::optional;
using std::string;
using std::stringstream;
using std::string_view;

using Shared::Model::CommandResult;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator""s;
#pragma warning(pop)

namespace DebugSymbolManager::Model {

    NtSymbolPath::NtSymbolPath(Shared::Services::IFileService const& fileService)
        : localCache(nullopt)
        , fileService(fileService) {

    }

    optional<string> const& NtSymbolPath::GetLocalCache() const noexcept {
        return localCache;
    }

    CommandResult NtSymbolPath::SetLocalCache(string const& value) noexcept {
        if (value.empty() || !fileService.DirectoryExists(value))
            return CommandResult::Fail("IDirectory not found");
        localCache = value;
        UpdateIsModified();
        return CommandResult::Ok();
    }

    optional<string> NtSymbolPath::GetSymbolPath() const noexcept {
        try {
            auto symbolPath = baseSymbolPath;
            if (auto const index = baseSymbolPath.find(Settings::LOCAL_CACHE_KEY); 
                index != string::npos) {

                if (auto const cache = localCache.value_or(""s); // change or with environmentService.GetTempPath
                    !cache.empty() && fileService.DirectoryExists(cache)) {
                    symbolPath = symbolPath.replace(index, Settings::LOCAL_CACHE_KEY_SIZE, cache);
                }
            }

            stringstream builder{};
            builder << symbolPath;

            for (auto const& path : additionalPaths)
                builder << ";" << path;

            return optional(builder.str());
        }
        catch (std::exception const&) {
            return nullopt;
        }
    }

    std::string const& NtSymbolPath::GetSymbolServer() const noexcept
    {
        return symbolServer;
    }

    void NtSymbolPath::SetSymbolServer(std::string server) noexcept {
        if (server == symbolServer)
            return;
        symbolServer = move(server);
    }

    void NtSymbolPath::AddDirectory(std::string const& directory) noexcept {
        if (directory.empty() || !fileService.DirectoryExists(directory))
            return;

        if (find(begin(additionalPaths), end(additionalPaths), directory) == end(additionalPaths))
            return;

        additionalPaths.emplace_back(directory);
        UpdateIsModified();
    }

    void NtSymbolPath::RemoveDirectory(std::string const& directory) noexcept {
        auto const& entry = find(begin(additionalPaths), end(additionalPaths), directory);
        if (entry == end(additionalPaths))
            return;
        additionalPaths.erase(entry);
        UpdateIsModified();
    }

    bool NtSymbolPath::IsModified() const noexcept {
        return isModified;
    }

    void NtSymbolPath::ResetModified() noexcept {
        lastSavedState = GetSymbolPath().value_or(""s);
        isModified = false;
    }
    void NtSymbolPath::UpdateIsModified() noexcept {
        isModified = lastSavedState != GetSymbolPath().value_or(""s) || lastSavedState == ""s;
    }


}