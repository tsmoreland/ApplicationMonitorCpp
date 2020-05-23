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

#include "shared/file_service.h"

namespace shared::service
{

    class file_service_impl final : public file_service
    {
    public:
        [[nodiscard]] SHARED_DLL std::vector<std::filesystem::path> get_files_from_directory(std::filesystem::path const& folder, std::wregex const& filter) const noexcept override;
        [[nodiscard]] SHARED_DLL bool directory_exists(std::string_view const path) const override;

        SHARED_DLL file_service_impl() = default;
        SHARED_DLL file_service_impl(const file_service_impl&) = default;
        SHARED_DLL file_service_impl(file_service_impl&&) noexcept = default;
        SHARED_DLL file_service_impl& operator=(const file_service_impl&) = default;
        SHARED_DLL file_service_impl& operator=(file_service_impl&&) noexcept = default;
        SHARED_DLL ~file_service_impl() override = default;
    };

    [[nodiscard]] inline shared_file_service make_shared_file_service()
    {
        return std::dynamic_pointer_cast<file_service>(std::make_shared<file_service_impl>());
    }

}
