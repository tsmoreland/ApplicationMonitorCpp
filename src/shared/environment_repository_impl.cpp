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

using std::nullopt;
using std::optional;
using std::string;

namespace shared::infrastructure
{
shared_const_environment_repository make_const_environment_repository()
{
    return std::make_shared<environment_repository_impl const>();
}

shared_environment_repository make_environment_repository()
{
    return std::make_shared<environment_repository_impl>();
}

optional<string> environment_repository_impl::get_variable(std::string const& key) const noexcept
{
    constexpr auto MAX_VARIABLE_NAME_SIZE = 8192;
    char value[MAX_VARIABLE_NAME_SIZE]{};

    auto const size = GetEnvironmentVariableA(key.c_str(), value, MAX_VARIABLE_NAME_SIZE);
    if (size > MAX_VARIABLE_NAME_SIZE || size == 0)
        return nullopt;
    return optional(string(value));
}
bool environment_repository_impl::set_variable(string const& key, string const& value) const noexcept
{
    return SetEnvironmentVariableA(key.c_str(), value.c_str()) == TRUE;
}

bool environment_repository_impl::remove_variable(string const& key) const noexcept
{
    return SetEnvironmentVariableA(key.c_str(), nullptr) == TRUE;

}

}
