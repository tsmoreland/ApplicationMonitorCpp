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

using std::copy;
using std::filesystem::path;
using std::initializer_list;
using std::move;
using std::optional;
using std::string;
using std::string_view;
using std::swap;
using std::unique_ptr;
using std::vector;
using std::wregex;

#define BOOST_TEST_MODULE symbol_path_service
#include <boost/test/included/unit_test.hpp>

#include <type_traits>
#include "test_adapter.h"

using symbol_manager::model::settings;
using symbol_manager::service::symbol_path_service_impl;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
#pragma warning(pop)

#include "context_builder.h"

using namespace symbol_manager::test;

BOOST_AUTO_TEST_CASE(update_application_path_changes_symbol_path)
{
    // arrange
    auto const app_path = R"(C:\Program Files\Application)"s;
    auto const expectedVariableValue = string(SYMBOL_SERVER) + ";"s + app_path;
    auto context = context_builder::arrange()
        .with_expected_set_calls(successfully_set_to(string(SYMBOL_SERVER) + ";"s + app_path, Exactly(1)))
        .with_existing_directories({app_path})
        .with_service_created()
        .Build();

    // Act
    static_cast<void>(context.service->update_application_path(app_path));

    // Assert
    // again handled by the arrange and specifically expected set calls
}

BOOST_AUTO_TEST_CASE(update_application_pathReturnsSuccess)
{
    // arrange
    auto const app_path = R"(C:\Program Files\Application)"s;
    auto const expectedVariableValue = string(SYMBOL_SERVER) + ";"s + app_path;
    auto context = context_builder::arrange()
        .with_expected_set_calls(successfully_set_to(string(SYMBOL_SERVER) + ";"s + app_path, Exactly(1)))
        .with_existing_directories({app_path})
        .with_service_created()
        .Build();

    // Act
    auto const result = context.service->update_application_path(app_path);

    // Assert
    BOOST_ASSERT(result.is_success());
}

BOOST_AUTO_TEST_CASE(UpdateReplacesOldApplicationPath)
{
    // arrange
    auto const app_path = R"(C:\Program Files\Application)"s;
    auto const replacementapp_path = R"(C:\Program Files (x86)\AlternateApplication)"s;
    auto const expectedVariableValue = string(SYMBOL_SERVER) + ";"s + app_path;
    auto const replacementExpectedVariableValue = string(SYMBOL_SERVER) + ";"s + replacementapp_path;

    auto context = context_builder::arrange()
        .with_expected_set_calls(successfully_set_to(expectedVariableValue, Exactly(1)), successfully_set_to(replacementExpectedVariableValue, Exactly(1)))
        .with_existing_directories({app_path, replacementapp_path})
        .with_service_created()
        .Build();
    static_cast<void>(context.service->update_application_path(app_path));

    // Act
    static_cast<void>(context.service->update_application_path(replacementapp_path));

    // Assert
    // again handled by the arrange and specifically expected set calls
}

BOOST_AUTO_TEST_CASE(UpdateApplicaitonPathWithReplacementReturnsSuccess)
{
    // arrange
    auto const app_path = R"(C:\Program Files\Application)"s;
    auto const replacementapp_path = R"(C:\Program Files (x86)\AlternateApplication)"s;
    auto const expectedVariableValue = string(SYMBOL_SERVER) + ";"s + app_path;
    auto const replacementExpectedVariableValue = string(SYMBOL_SERVER) + ";"s + replacementapp_path;

    auto context = context_builder::arrange()
        .with_expected_set_calls(successfully_set_to(expectedVariableValue, Exactly(1)), successfully_set_to(replacementExpectedVariableValue, Exactly(1)))
        .with_existing_directories({app_path, replacementapp_path})
        .with_service_created()
        .Build();
    static_cast<void>(context.service->update_application_path(app_path));

    // Act
    auto const result = context.service->update_application_path(replacementapp_path);

    // Assert
    BOOST_ASSERT(result.is_success());
}

BOOST_AUTO_TEST_CASE(update_application_pathFailsIfDirectoryDoesNotExist)
{
    // arrange
    auto const app_path = R"(C:\Program Files\Application)"s;
    auto const expectedVariableValue = string(SYMBOL_SERVER) + ";"s + app_path;
    auto context = context_builder::arrange()
        .with_service_created()
        .Build();

    // Act
    auto const result = context.service->update_application_path(app_path);

    // Assert
    BOOST_ASSERT(!result.is_success());
}
