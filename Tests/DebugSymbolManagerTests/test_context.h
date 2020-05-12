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

#include "mock_objects.h"
#include "expected_set_call.h"

namespace debug_symbol_manager::test
{
    constexpr auto SYMBOL_PATH_VAR = "_NT_SYMBOL_PATH";
    constexpr auto SYMBOL_SERVER = "*SRV";

    struct test_context
    {
        test_context()
        {
            repository = make_unique<mock_objects::mock_environment_repository>();
            file_service = make_unique<mock_objects::mock_file_service>();
        }
        test_context(test_context const&) = delete;
        test_context(test_context&& other) noexcept
            : repository(other.repository.release())
            , file_service(other.file_service.release())
            , service(other.service.release())
            , application_path(move(other.application_path))
            , initial_symbol_path(move(other.initial_symbol_path))
            , expected_symbol_path(move(other.expected_symbol_path))
            , number_of_get_calls(move(other.number_of_get_calls))
            , expected_set_calls(move(other.expected_set_calls)) 
            , existing_directories(move(other.existing_directories))
        {
            other.reset();
        }
        test_context& operator=(test_context const& other) = delete;
        test_context& operator=(test_context&& other) noexcept
        {
            if (this == &other)
                return *this;

            ::swap(repository, other.repository);
            ::swap(file_service, other.file_service);
            ::swap(application_path, other.application_path);
            ::swap(initial_symbol_path, other.initial_symbol_path);
            ::swap(expected_symbol_path, other.expected_symbol_path);
            ::swap(number_of_get_calls, other.number_of_get_calls);
            ::swap(expected_set_calls, other.expected_set_calls);
            ::swap(service, other.service);
            ::swap(existing_directories, other.existing_directories);

            other.reset();
            return *this;
        }
        ~test_context() = default;

        void reset() noexcept
        {
            repository = make_unique<mock_objects::mock_environment_repository>();
            file_service = make_unique<mock_objects::mock_file_service>();
            settings = ::settings{SYMBOL_SERVER};
            number_of_get_calls = AnyNumber();
            expected_set_calls.clear();
            existing_directories.clear();
        }

        std::unique_ptr<mock_objects::mock_environment_repository> repository{};
        std::unique_ptr<mock_objects::mock_file_service> file_service{};
        std::unique_ptr<symbol_path_service> service{};

        settings settings{SYMBOL_SERVER};
        std::string application_path{};
        std::string initial_symbol_path{};
        std::string expected_symbol_path{};
        Cardinality number_of_get_calls{AnyNumber()};
        vector<expected_set_call> expected_set_calls{};
        vector<string> existing_directories{};
    };

}
