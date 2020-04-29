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

using DebugSymbolManager::Model::Settings;
using DebugSymbolManager::Service::ISymbolPathService;
using DebugSymbolManager::Service::SymbolPathService;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
#pragma warning(pop)

namespace Test {
    class MockEnviromentRepository : public Shared::Infrastructure::IEnvironmentRepository {
    public:
        MOCK_METHOD(optional<string>, GetVariable, (string const& key), (const, noexcept, override));
        MOCK_METHOD(bool, SetVariable, (string const& key, string const& value), (const, noexcept, override));
        MOCK_METHOD(bool, RemoveVariable, (string const& key), (const, noexcept, override));
    };
    class MockFileService : public Shared::Service::IFileService {
    public:
        MOCK_METHOD(vector<path>, GetFilesFromDirectory, (path const& folder, wregex const& filter), (const, noexcept, override));
        MOCK_METHOD(bool, DirectoryExists, (std::string_view const path), (const, override));

    };
}


BOOST_AUTO_TEST_CASE(ConstructorGetsCurrentSymbolPath) {
    // Arrange
    Test::MockEnviromentRepository enviromentRepository;
    Test::MockFileService fileService;
    Settings settings{ "local_cache"s, "base_[Local Cache]_template"s};

    // Act
    SymbolPathService service(settings, enviromentRepository, fileService);
    
    // Assert -- covered by expect call?

}

BOOST_AUTO_TEST_CASE(Sample) {

  BOOST_TEST(1 == 1);
  BOOST_TEST(true);
}


