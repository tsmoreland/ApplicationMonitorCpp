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

#include "../../Shared/environment_repository.h"
#include "../../Shared/file_service.h"

namespace mock_objects
{
    class mock_environment_repository final : public shared::infrastructure::environment_repository
    {
    public:
        MOCK_METHOD(optional<string>, get_variable, (string const& key), (const, noexcept, override));
        MOCK_METHOD(bool, set_variable, (string const& key, string const& value), (const, noexcept, override));
        MOCK_METHOD(bool, remove_variable, (string const& key), (const, noexcept, override));
    };

    class mock_file_service final : public shared::service::file_service
    {
    public:
        MOCK_METHOD(vector<path>, get_files_from_directory, (path const& folder, wregex const& filter), (const, noexcept, override));
        MOCK_METHOD(bool, directory_exists, (std::string_view const path), (const, override));

    };
}

