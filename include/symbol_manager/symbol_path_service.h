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

#include <string>
#include "common.h"
#include "command_result.h"

namespace debug_symbol_manager::service
{
    struct symbol_path_service
    {
        [[nodiscard]] DEBUG_SYMBOL_MANAGER_DLL virtual shared::model::command_result update_application_path(std::string const& application_path) noexcept = 0;
        DEBUG_SYMBOL_MANAGER_DLL virtual void reload() const noexcept = 0;

        DEBUG_SYMBOL_MANAGER_DLL symbol_path_service() = default;
        DEBUG_SYMBOL_MANAGER_DLL symbol_path_service(symbol_path_service const&) = default;
        DEBUG_SYMBOL_MANAGER_DLL symbol_path_service(symbol_path_service&&) noexcept = default;
        DEBUG_SYMBOL_MANAGER_DLL virtual ~symbol_path_service() = default;

        DEBUG_SYMBOL_MANAGER_DLL symbol_path_service& operator=(symbol_path_service const&) = default;
        DEBUG_SYMBOL_MANAGER_DLL symbol_path_service& operator=(symbol_path_service&&) noexcept = default;
    };

    using shared_symbol_path_service = std::shared_ptr<symbol_path_service>;
    using unique_symbol_path_service = std::unique_ptr<symbol_path_service>;

}
