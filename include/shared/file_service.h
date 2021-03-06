//
// Copyright � 2020 Terry Moreland
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
#include <regex>
#include <vector>
#include "shared/shared_export.h"

namespace shared::service
{
    struct file_service
    {
        [[nodiscard]] SHARED_DLL virtual std::vector<std::filesystem::path> get_files_from_directory(std::filesystem::path const& folder, std::wregex const& filter) const noexcept = 0;
        [[nodiscard]] SHARED_DLL virtual bool directory_exists(std::string_view const path) const = 0;

        file_service() = default;
        virtual ~file_service() = default;
        file_service(file_service&&) noexcept = default;
        file_service(file_service const&) = default;
        file_service& operator=(file_service&&) noexcept = default;
        file_service& operator=(file_service const&) = default;
    };

    using shared_file_service = std::shared_ptr<file_service>;
    using shared_const_file_service = std::shared_ptr<file_service const>;

    using unique_file_service = std::unique_ptr<file_service>;
    using unique_const_file_service = std::unique_ptr<file_service const>;

    [[nodiscard]] SHARED_DLL shared_file_service make_file_service();
    [[nodiscard]] SHARED_DLL shared_const_file_service make_const_file_service();

    [[nodiscard]] SHARED_DLL unique_file_service make_unique_file_service();
    [[nodiscard]] SHARED_DLL unique_const_file_service make_unique_const_file_service();

}
