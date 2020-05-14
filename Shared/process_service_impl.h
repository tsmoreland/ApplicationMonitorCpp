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

#include "process_service.h"
#include "export.h"

namespace shared::service {

    class process_service_impl final : public process_service {
    public:
        [[nodiscard]] SHARED_DLL unique_process start_process(std::string_view const& filename, std::string_view const& arguments) const noexcept override;
        [[nodiscard]] SHARED_DLL std::vector<unique_process> get_processes_by_name(std::string_view const& process_name) const noexcept override;
        [[nodiscard]] SHARED_DLL std::optional<std::filesystem::path> get_path_to_running_process(std::string_view const& process_name) const noexcept override;

        SHARED_DLL process_service_impl() = default;
        SHARED_DLL process_service_impl(const process_service_impl&) = default;
        SHARED_DLL process_service_impl(process_service_impl&&) noexcept = default;
        SHARED_DLL process_service_impl& operator=(const process_service_impl&) = default;
        SHARED_DLL process_service_impl& operator=(process_service_impl&&) noexcept = default;
        SHARED_DLL ~process_service_impl() override = default;
    };

    [[nodiscard]] inline shared_process_service make_shared_process_service()
    {
        return std::dynamic_pointer_cast<process_service>(std::make_shared<process_service_impl>());
    }

}
