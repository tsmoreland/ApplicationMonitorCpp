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

using std::string;

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

    CommandResult SymbolPathService::UpdateApplicationPath(string const& applicationPath) noexcept {
        try {
            if (m_applicationPath == applicationPath)
                return CommandResult::Ok("No update required");

            m_symbolPath.RemoveDirectory(m_applicationPath);
            
            if (m_fileService.DirectoryExists(applicationPath))
                m_symbolPath.AddDirectory(applicationPath);

            m_applicationPath = applicationPath;
            UpdateIfModified();

            return CommandResult::Ok();

        } catch (std::exception const& ex) {
            return CommandResult::Fail(ex.what());
        }
    }


    void SymbolPathService::Reload() const noexcept {
        UpdateIfModified();
    }

    SymbolPathService::SymbolPathService(Settings const& settings, IEnvironmentRepository const& environemntRepository, IFileService const& fileService)
        : m_environemntRepository(environemntRepository)
        , m_symbolPath{fileService}
        , m_fileService(fileService) {

        m_symbolPath.SetBaseSymbolPath(settings.BaseSymbolPath);
        if (!m_symbolPath.Reset(environemntRepository.GetVariable(NtSymbolPath::ENVIRONMENT_KEY).value_or(""s)).IsSuccess()) {
            // Log
        }

        UpdateIfModified();
    }

    void SymbolPathService::UpdateIfModified() const noexcept {
        if (auto const updatedPath = m_symbolPath.GetSymbolPath(); 
            m_symbolPath.IsModified() && updatedPath.has_value())

            if (auto const updated = m_environemntRepository.SetVariable(NtSymbolPath::ENVIRONMENT_KEY, updatedPath.value()); 
                !updated) {
                // TODO: log 
            }
    }

}
