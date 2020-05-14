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
#include "nt_symbol_path.h"
#include <sstream>

using std::filesystem::path;
using std::find;
using std::move;
using std::nullopt;
using std::optional;
using std::string;
using std::stringstream;
using std::string_view;

using shared::model::command_result;
using collection::contains;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator""s;
#pragma warning(pop)

namespace debug_symbol_manager::model
{

nt_symbol_path::nt_symbol_path(shared::service::shared_const_file_service const& file_service)
    : m_file_service(file_service)
{
}

optional<string> nt_symbol_path::get_symbol_path() const noexcept
{
    try {
        stringstream builder{};

        if (m_base_symbol_path != ""s)
            builder << m_base_symbol_path;

        for (auto const& path : m_additional_paths) {
            builder << ";" << path;
        }

        return optional(builder.str());
    }
    catch (std::exception const&) {
        return nullopt;
    }
}

std::string const& nt_symbol_path::get_base_symbol_path() const noexcept
{
    return m_base_symbol_path;
}

void nt_symbol_path::set_base_symbol_path(std::string const& server)
{
    if (server == m_base_symbol_path)
        return;
    m_base_symbol_path = server;
    update_is_modified();
}

command_result nt_symbol_path::add_directory(std::string const& directory) noexcept
{
    try {
        if (directory.empty() || !m_file_service->directory_exists(directory))
            return command_result::fail("Directory not found");

        if (contains(m_additional_paths, directory))
            return command_result::ok("Already present");

        m_additional_paths.emplace_back(directory);
        update_is_modified();
        return command_result::ok();

    } catch (std::exception const& ex) {
        return command_result::error(ex);
    }
}

void nt_symbol_path::remove_directory(std::string const& directory) noexcept
{
    auto const& entry = find(begin(m_additional_paths), end(m_additional_paths), directory);
    if (entry == end(m_additional_paths))
        return;
    m_additional_paths.erase(entry);
    update_is_modified();
}

bool nt_symbol_path::is_modified() const noexcept
{
    return m_is_modified;
}

command_result nt_symbol_path::reset(string const& currentValue) noexcept
{
    try {
        m_last_saved_state = currentValue;
        update_is_modified();
        return command_result::ok();

    } catch (std::exception const& ex) {
        return command_result::error(ex);
    }
}
void nt_symbol_path::update_is_modified() noexcept
{
    m_is_modified = m_last_saved_state != get_symbol_path().value_or(""s) || m_last_saved_state == ""s;
}

}