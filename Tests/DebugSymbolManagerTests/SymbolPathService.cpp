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

using std::filesystem::path;
using std::optional;
using std::string;
using std::vector;
using std::wregex;

#define BOOST_TEST_MODULE SymbolPathService
#include <boost/test/included/unit_test.hpp>

#include "TestAdapter.h"
#include "TestFixture.h"

using DebugSymbolManager::Model::Settings;
using DebugSymbolManager::Service::ISymbolPathService;
using DebugSymbolManager::Service::SymbolPathService;

#include "MockObjects.h"

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
#pragma warning(pop)

BOOST_AUTO_TEST_CASE(ConstructorGetsCurrentSymbolPath) {
    // Arrange
    MockObjects::MockEnviromentRepository enviromentRepository;
    MockObjects::MockFileService fileService;
    Settings settings{ "*SRV"s};

    EXPECT_CALL(enviromentRepository, GetVariable("_NT_SYMBOL_PATH"))
        .Times(1)
        .WillRepeatedly(Return(optional("sympath123")));

    // Act
    SymbolPathService service(settings, enviromentRepository, fileService);
    
    // Assert -- covered by expect call
}


BOOST_AUTO_TEST_CASE(ConstructorUpdatesCurrentSymbolPathWhenHasValue) {
    // Arrange
    MockObjects::MockEnviromentRepository enviromentRepository;
    MockObjects::MockFileService fileService;
    Settings settings{  "*SRV"s};

    EXPECT_CALL(enviromentRepository, GetVariable("_NT_SYMBOL_PATH"))
        .Times(1)
        .WillRepeatedly(Return(optional("sympath123")));

    // Act
    SymbolPathService service(settings, enviromentRepository, fileService);
    
    // Assert -- covered by expect call

}
