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

#include "ISymbolPathService.h"
#include "Settings.h"
#include "NtSymbolPath.h"
#include <IEnvironmentRepository.h>
#include <IFileService.h>

namespace DebugSymbolManager::Service {

    class SymbolPathService final : public ISymbolPathService {
    public:
        DEBUG_SYMBOL_MANAGER_DLL Shared::Model::CommandResult UpdateApplicationPath(std::filesystem::path const& applicationPath) noexcept override;

        DEBUG_SYMBOL_MANAGER_DLL explicit SymbolPathService(DebugSymbolManager::Model::Settings const& settings, Shared::Infrastructure::IEnvironmentRepository const& environemntRepository, Shared::Service::IFileService const& fileService);
        DEBUG_SYMBOL_MANAGER_DLL SymbolPathService(SymbolPathService const&) = default;
        DEBUG_SYMBOL_MANAGER_DLL SymbolPathService(SymbolPathService&&) noexcept = default;
        DEBUG_SYMBOL_MANAGER_DLL ~SymbolPathService() override = default;
        DEBUG_SYMBOL_MANAGER_DLL SymbolPathService& operator=(SymbolPathService const&) = delete;
        DEBUG_SYMBOL_MANAGER_DLL SymbolPathService& operator=(SymbolPathService&&) noexcept = delete;
    private:
        Shared::Infrastructure::IEnvironmentRepository const& environemntRepository;
        DebugSymbolManager::Model::NtSymbolPath symbolPath;
        std::filesystem::path applicationSymbolFolder;

        void UpdateIfModified() const noexcept;
    };

}
