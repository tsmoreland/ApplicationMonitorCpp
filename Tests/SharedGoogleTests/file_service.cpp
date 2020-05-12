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
#include <file_service_impl.h>
#include "Common.h"

using std::filesystem::directory_entry;
using std::filesystem::path;
using std::make_unique;
using std::regex_match;
using std::tie;
using std::tuple;
using std::unique_ptr;
using std::vector;
using std::wregex;

using shared::service::file_service;
using shared::service::file_service_impl;

namespace Shared::FileServiceTests {

    template <class PREDICATE>
    tuple<unique_ptr<file_service>, vector<path>> Arrange(path const& folder, PREDICATE predicate);

    TEST(file_service, returns_no_files_when_path_is_not_directory)
    {
        // Arrange
        auto const windows_directory = path(LR"(C:\windows\system32\cmd.exe)");
        wregex const filter(LR"(.*\.exe$)");
        unique_ptr<file_service> const service(make_unique<file_service_impl>());

        // Act
        auto const files = service->get_files_from_directory(windows_directory, filter);

        // Assert
        ASSERT_EQ(0ULL, files.size());
    }

    TEST(file_service, returns_all_files_matching_filter)
    {
        // Arrange
        auto const windows_directory = path(LR"(C:\windows)");
        unique_ptr<file_service> service;
        vector<path> expected;
        wregex const filter(LR"(.*\.exe$)");
        tie(service, expected) = Arrange(windows_directory, 
            [&filter](directory_entry const& entry) {
               return regex_match(entry.path().wstring(), filter);
            });

        // Act
        auto const files = service->get_files_from_directory(windows_directory, filter);

        // Assert
        ASSERT_TRUE(equal(begin(expected), end(expected), begin(files)));
    }



    template <class PREDICATE>
    tuple<unique_ptr<file_service>, vector<path>> Arrange(path const& folder, PREDICATE predicate)
    {
        unique_ptr<file_service> service = make_unique<file_service_impl>();
        return tuple<unique_ptr<file_service>, vector<path>>(service.release(), Tests::PopulateExpectedFiles(folder, predicate));
    }
}
