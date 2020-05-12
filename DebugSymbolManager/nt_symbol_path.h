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
#include "settings.h"
#include "file_service.h"
#include <command_result.h>

namespace debug_symbol_manager::model
{
    class nt_symbol_path final
    {
    public:
        [[nodiscard]] std::optional<std::string> get_symbol_path() const noexcept;

        [[nodiscard]] std::string const& get_base_symbol_path() const noexcept;
        void set_base_symbol_path(std::string const& server) noexcept;

        [[nodiscard]] shared::model::command_result add_directory(std::string const& directory) noexcept;
        void remove_directory(std::string const& directory) noexcept;

        [[nodiscard]] bool is_modified() const noexcept;
        [[nodiscard]] shared::model::command_result reset(std::string const& currentValue) noexcept;

        explicit nt_symbol_path(shared::service::file_service const& file_service);
        nt_symbol_path(nt_symbol_path const&) = default;
        nt_symbol_path(nt_symbol_path&&) noexcept = default;
        ~nt_symbol_path() = default;

        nt_symbol_path& operator=(nt_symbol_path const&) = delete;
        nt_symbol_path& operator=(nt_symbol_path&&) = delete;

        constexpr static auto ENVIRONMENT_KEY = "_NT_SYMBOL_PATH";
    private:
        std::string m_last_saved_state{};
        bool m_is_modified{false};
        std::string m_base_symbol_path{};
        shared::service::file_service const& m_file_service;
        std::vector<std::string> m_additional_paths{};

        void update_is_modified() noexcept;
    };
}
