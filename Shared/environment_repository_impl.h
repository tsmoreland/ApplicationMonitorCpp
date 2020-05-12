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
#include "environment_repository.h"

namespace shared::infrastructure {

    class environment_repository_impl final : public environment_repository
    {
    public:
        [[nodiscard]] SHARED_DLL std::optional<std::string> get_variable(std::string const& key) const noexcept override;
        [[nodiscard]] SHARED_DLL bool set_variable(std::string const& key, std::string const& value) const noexcept override;
        [[nodiscard]] SHARED_DLL virtual bool remove_variable(std::string const& key) const noexcept override;

        SHARED_DLL environment_repository_impl() = default;
        SHARED_DLL environment_repository_impl(const environment_repository_impl&) = default;
        SHARED_DLL environment_repository_impl(environment_repository_impl&&) noexcept = default;
        SHARED_DLL environment_repository_impl& operator=(const environment_repository_impl&) = default;
        SHARED_DLL environment_repository_impl& operator=(environment_repository_impl&&) noexcept = default;
        SHARED_DLL ~environment_repository_impl() override = default;
    };

    inline shared_environemnt_repository make_shared_environemnt_repository()
    {
        return std::dynamic_pointer_cast<environment_repository>(std::make_shared<environment_repository_impl>());
    }

}
