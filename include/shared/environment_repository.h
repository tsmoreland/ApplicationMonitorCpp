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
#include <string>
#include "shared/shared_export.h"

namespace shared::infrastructure
{
    struct environment_repository
    {
        [[nodiscard]] SHARED_DLL virtual std::optional<std::string> get_variable(std::string const& key) const noexcept = 0;
        [[nodiscard]] SHARED_DLL virtual bool set_variable(std::string const& key, std::string const& value) const noexcept = 0;
        [[nodiscard]] SHARED_DLL virtual bool remove_variable(std::string const& key) const noexcept = 0;

        virtual ~environment_repository() = default;
        environment_repository() = default;
        environment_repository(environment_repository&&) noexcept = default;
        environment_repository(environment_repository const&) = default;

        environment_repository& operator=(environment_repository&&) noexcept = default;
        environment_repository& operator=(environment_repository const&) = default;
    };

    using shared_environment_repository = std::shared_ptr<environment_repository>;
    using shared_const_environment_repository = std::shared_ptr<environment_repository const>;

    using unique_environemnt_repository = std::unique_ptr<environment_repository>;
    using unique_const_environemnt_repository = std::unique_ptr<environment_repository const>;

    [[nodiscard]] SHARED_DLL shared_const_environment_repository make_const_environment_repository();
    [[nodiscard]] SHARED_DLL shared_environment_repository make_environment_repository();

}
