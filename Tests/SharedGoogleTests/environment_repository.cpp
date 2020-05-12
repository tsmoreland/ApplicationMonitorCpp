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

#include "pch.h"
#include "environment_repository_impl.h"

using shared::infrastructure::environment_repository_impl;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
using std::literals::chrono_literals::operator ""s;
#pragma warning(pop)

namespace Shared::Tests
{
TEST(environment_repository, path_variable_has_value)
{
    environment_repository_impl const repository{};
    auto const path_variable = repository.get_variable("PATH");
    ASSERT_FALSE(path_variable->empty());
}

TEST(environment_repository, VariableIsUpdated)
{
    environment_repository_impl const repository{};
    auto const key = "SET_ENV_TEST"s;

    EXPECT_TRUE(repository.set_variable(key, "ALPHA"s));
    EXPECT_TRUE(repository.get_variable(key) == "ALPHA"s);

    ASSERT_TRUE(repository.set_variable(key, "BETA"s));
    ASSERT_TRUE(repository.get_variable(key) == "BETA"s);

    static_cast<void>(repository.remove_variable(key));
}

TEST(environment_repository, variable_is_removed)
{
    environment_repository_impl const repository{};

    auto const key = "DEL_ENV_TEST"s;

    static_cast<void>(repository.remove_variable(key));
    EXPECT_FALSE(repository.get_variable(key).has_value());

    EXPECT_TRUE(repository.set_variable(key, "ALPHA"s));
    EXPECT_TRUE(repository.get_variable(key) == "ALPHA"s);

    ASSERT_TRUE(repository.remove_variable(key));
}

}

