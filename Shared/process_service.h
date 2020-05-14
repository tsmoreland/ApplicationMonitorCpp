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

#include <filesystem>
#include <optional>
#include <vector>
#include <regex>
#include "process.h"
#include "export.h"

namespace shared::service
{
    struct process_service
    {
        using unique_process = shared::model::unique_process;

        [[nodiscard]] SHARED_DLL virtual unique_process start_process(std::string_view const& filename, std::string_view const& arguments) const noexcept = 0;
        [[nodiscard]] SHARED_DLL virtual std::vector<unique_process> get_processes_by_name(std::string_view const& processName) const noexcept = 0;
        [[nodiscard]] SHARED_DLL virtual std::optional<std::filesystem::path> get_path_to_running_process(std::string_view const& processName) const noexcept = 0;

        process_service() = default;
        virtual ~process_service() = default;
        process_service(process_service&&) noexcept = default;
        process_service(process_service const&) = default;
        process_service& operator=(process_service&&) noexcept = default;
        process_service& operator=(process_service const&) = default;
    };

    using shared_process_service = std::shared_ptr<process_service>;
}

