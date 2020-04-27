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
#include "EnvironmentRepository.h"

using std::stringstream;
using std::to_string;

using Shared::Infrastructure::EnvironmentRepository;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
using std::literals::chrono_literals::operator ""s;
#pragma warning(pop)

namespace Shared::Tests {


    TEST(EnvironmentRepository, PathVariableHasValue) {

        EnvironmentRepository const repository{};

        auto const pathVariable = repository.GetVariable("PATH");

        ASSERT_FALSE(pathVariable->empty());
    }

    TEST(EnvironmentRepository, VariableIsUpdated) {
        EnvironmentRepository const repository{};
        auto const key = "ENV_TEST"s;
        auto const start = repository.GetVariable(key);

        ASSERT_TRUE(repository.SetVariable(key, "ALPHA"s));
        ASSERT_TRUE(repository.GetVariable(key) == "ALPHA"s);

        ASSERT_TRUE(repository.SetVariable(key, "BETA"s));
        ASSERT_TRUE(repository.GetVariable(key) == "BETA"s);
    }

}

