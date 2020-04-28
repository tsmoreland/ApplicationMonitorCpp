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
#include "SymbolPathService.h"

namespace filesystem = std::filesystem;
using std::filesystem::path;

using DebugSymbolManager::Model::Settings;
using DebugSymbolManager::Model::NtSymbolPath;
using Shared::Infrastructure::IEnvironmentRepository;
using Shared::Model::CommandResult;
using Shared::Service::IFileService;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
using std::literals::chrono_literals::operator ""s;
#pragma warning(pop)

namespace DebugSymbolManager::Service {

    CommandResult SymbolPathService::UpdateApplicationPath(path const& applicationPath) noexcept {
        try {
            if (this->applicationSymbolFolder == applicationPath)
                return CommandResult::Ok("No update required");

            symbolPath.RemoveDirectory(this->applicationSymbolFolder.generic_string());
            if (filesystem::exists(applicationPath) && filesystem::is_directory(applicationPath))
                symbolPath.AddDirectory(applicationPath.generic_string());

            this->applicationSymbolFolder = applicationPath;
            UpdateIfModified();

            return CommandResult::Ok();

        } catch (std::exception const& ex) {
            return CommandResult::Fail(ex.what());
        }
    }

    SymbolPathService::SymbolPathService(Settings const& settings, IEnvironmentRepository const& environemntRepository, IFileService const& fileService)
        : environemntRepository(environemntRepository)
        , symbolPath(fileService) {

        if (auto const result = symbolPath.SetLocalCache(settings.LocalCache); !result) {
            // TODO: add logging
        }

        symbolPath.SetSymbolServer(settings.BaseSymbolPath);
        symbolPath.Reset(environemntRepository.GetVariable(NtSymbolPath::ENVIRONMENT_KEY).value_or(""s));

        UpdateIfModified();
    }

    void SymbolPathService::UpdateIfModified() const noexcept {
        if (auto const updatedPath = symbolPath.GetSymbolPath(); symbolPath.IsModified() && updatedPath.has_value())
            if (auto const updated = environemntRepository.SetVariable(NtSymbolPath::ENVIRONMENT_KEY, updatedPath.value()); !updated) {
                // TODO: log 
            }
    }

}
