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

#define BOOST_TEST_MODULE SymbolPathService
#include <boost/test/included/unit_test.hpp>

#include <type_traits>
#include "TestAdapter.h"
#include "TestFixture.h"

using DebugSymbolManager::Model::Settings;
using DebugSymbolManager::Service::ISymbolPathService;
using DebugSymbolManager::Service::SymbolPathService;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
#pragma warning(pop)

#include "ContextBuilder.h"

using namespace DebugSymbolManager::Test;

BOOST_AUTO_TEST_CASE(UpdateApplicationPathChangesSymbolPath) {
    // Arrange
    auto const appPath = R"(C:\Program Files\Application)"s;
    auto const expectedVariableValue = string(SYMBOL_SERVER) + ";"s + appPath;
    auto context = ContextBuilder::Arrange()
        .WithExpectedSetCalls(SuccessfullySetTo(string(SYMBOL_SERVER) + ";"s + appPath, Exactly(1)))
        .WithExistingDirectories({appPath})
        .WithServiceCreated()
        .Build();

    // Act
    static_cast<void>(context.Service->UpdateApplicationPath(appPath));

    // Assert
    // again handled by the arrange and specifically expected set calls
}

BOOST_AUTO_TEST_CASE(UpdateApplicationPathReturnsSuccess) {
    // Arrange
    auto const appPath = R"(C:\Program Files\Application)"s;
    auto const expectedVariableValue = string(SYMBOL_SERVER) + ";"s + appPath;
    auto context = ContextBuilder::Arrange()
        .WithExpectedSetCalls(SuccessfullySetTo(string(SYMBOL_SERVER) + ";"s + appPath, Exactly(1)))
        .WithExistingDirectories({appPath})
        .WithServiceCreated()
        .Build();

    // Act
    auto const result = context.Service->UpdateApplicationPath(appPath);

    // Assert
    BOOST_ASSERT(result.IsSuccess());
}

BOOST_AUTO_TEST_CASE(UpdateReplacesOldApplicationPath) {
    // Arrange
    auto const appPath = R"(C:\Program Files\Application)"s;
    auto const replacementAppPath = R"(C:\Program Files (x86)\AlternateApplication)"s;
    auto const expectedVariableValue = string(SYMBOL_SERVER) + ";"s + appPath;
    auto const replacementExpectedVariableValue = string(SYMBOL_SERVER) + ";"s + replacementAppPath;

    auto context = ContextBuilder::Arrange()
        .WithExpectedSetCalls(SuccessfullySetTo(expectedVariableValue, Exactly(1)), SuccessfullySetTo(replacementExpectedVariableValue, Exactly(1)))
        .WithExistingDirectories({appPath, replacementAppPath})
        .WithServiceCreated()
        .Build();
    static_cast<void>(context.Service->UpdateApplicationPath(appPath));

    // Act
    static_cast<void>(context.Service->UpdateApplicationPath(replacementAppPath));

    // Assert
    // again handled by the arrange and specifically expected set calls
}

BOOST_AUTO_TEST_CASE(UpdateApplicaitonPathWithReplacementReturnsSuccess) {
    // Arrange
    auto const appPath = R"(C:\Program Files\Application)"s;
    auto const replacementAppPath = R"(C:\Program Files (x86)\AlternateApplication)"s;
    auto const expectedVariableValue = string(SYMBOL_SERVER) + ";"s + appPath;
    auto const replacementExpectedVariableValue = string(SYMBOL_SERVER) + ";"s + replacementAppPath;

    auto context = ContextBuilder::Arrange()
        .WithExpectedSetCalls(SuccessfullySetTo(expectedVariableValue, Exactly(1)), SuccessfullySetTo(replacementExpectedVariableValue, Exactly(1)))
        .WithExistingDirectories({appPath, replacementAppPath})
        .WithServiceCreated()
        .Build();
    static_cast<void>(context.Service->UpdateApplicationPath(appPath));

    // Act
    auto const result = context.Service->UpdateApplicationPath(replacementAppPath);

    // Assert
    BOOST_ASSERT(result.IsSuccess());
}

BOOST_AUTO_TEST_CASE(UpdateApplicationPathFailsIfDirectoryDoesNotExist) {
    // Arrange
    auto const appPath = R"(C:\Program Files\Application)"s;
    auto const expectedVariableValue = string(SYMBOL_SERVER) + ";"s + appPath;
    auto context = ContextBuilder::Arrange()
        .WithServiceCreated()
        .Build();

    // Act
    auto const result = context.Service->UpdateApplicationPath(appPath);

    // Assert
    BOOST_ASSERT(!result.IsSuccess());
}
