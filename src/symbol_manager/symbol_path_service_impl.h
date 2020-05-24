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

#include <symbol_manager/symbol_path_service.h>
#include <symbol_manager/settings.h>
#include <symbol_manager/nt_symbol_path.h>
#include <shared/environment_repository.h>
#include <shared/file_service.h>
#include <string>

namespace symbol_manager::service
{
    class symbol_path_service_impl final : public symbol_path_service
    {
    public:
        [[nodiscard]] SYMBOL_MANAGER_DLL shared::model::command_result update_application_path(std::string const& application_path) noexcept override;
        SYMBOL_MANAGER_DLL virtual void reload() const noexcept override;

        SYMBOL_MANAGER_DLL explicit symbol_path_service_impl(symbol_manager::model::settings const& settings, shared::infrastructure::shared_const_environment_repository const& environemnt_repository, shared::service::shared_const_file_service const& file_service);
        SYMBOL_MANAGER_DLL symbol_path_service_impl(symbol_path_service_impl const&) = default;
        SYMBOL_MANAGER_DLL symbol_path_service_impl(symbol_path_service_impl&&) noexcept = default;
        SYMBOL_MANAGER_DLL ~symbol_path_service_impl() override = default;
        SYMBOL_MANAGER_DLL symbol_path_service_impl& operator=(symbol_path_service_impl const&) = delete;
        SYMBOL_MANAGER_DLL symbol_path_service_impl& operator=(symbol_path_service_impl&&) noexcept = delete;
    private:
        shared::infrastructure::shared_const_environment_repository m_environment_repository;
        symbol_manager::model::nt_symbol_path m_symbol_path;
        std::string m_application_path;
        shared::service::shared_const_file_service m_file_service;

        void update_if_modified() const noexcept;
    };

}
