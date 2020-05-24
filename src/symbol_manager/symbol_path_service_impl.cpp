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
#include "symbol_path_service_impl.h"

using std::string;

using symbol_manager::model::settings;
using symbol_manager::model::nt_symbol_path;
using shared::infrastructure::shared_const_environment_repository;
using shared::model::command_result;
using shared::service::shared_const_file_service;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
using std::literals::chrono_literals::operator ""s;
#pragma warning(pop)

namespace symbol_manager::service
{

command_result symbol_path_service_impl::update_application_path(string const& application_path) noexcept
{
    try {
        if (m_application_path == application_path)
            return command_result::ok("No update required");

        if (!m_file_service->directory_exists(application_path))
            return command_result::fail("path not found");

        m_symbol_path.remove_directory(m_application_path);
        
        if (!static_cast<bool>(m_symbol_path.add_directory(application_path))) {
            // Log error here
        }

        m_application_path = application_path;
        update_if_modified();

        return command_result::ok();

    } catch (std::exception const& ex) {
        return command_result::fail(ex.what());
    }
}


void symbol_path_service_impl::reload() const noexcept
{
    update_if_modified();
}

symbol_path_service_impl::symbol_path_service_impl(settings const& settings, shared_const_environment_repository const& environemnt_repository, shared_const_file_service const& file_service)
    : m_environment_repository(environemnt_repository)
    , m_symbol_path{file_service}
    , m_file_service(file_service) {

    if (!m_environment_repository)
        throw std::invalid_argument("environment_repository is null");

    if (!m_file_service)
        throw std::invalid_argument("file_service is null");

    if (!m_symbol_path.reset(environemnt_repository->get_variable(nt_symbol_path::ENVIRONMENT_KEY).value_or(""s)).is_success()) {
        // Log
    }

    m_symbol_path.set_base_symbol_path(settings.base_symbol_path);
    update_if_modified();
}

void symbol_path_service_impl::update_if_modified() const noexcept
{
    try {
        if (auto const updated_path = m_symbol_path.get_symbol_path(); 
            m_symbol_path.is_modified() && updated_path.has_value()) {

            if (auto const updated = m_environment_repository->set_variable(nt_symbol_path::ENVIRONMENT_KEY, updated_path.value()); 
                !updated) {
                // TODO: log 
            }
        }
    }
    catch (std::bad_optional_access const&) {
        // should never occur because we check has_value() first but here to silence warning
    }
}

}
