//
// Copyright � 2020 Terry Moreland
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
#include "common.h"

using std::filesystem::directory_entry;
using std::filesystem::path;
using std::regex_match;
using std::tie;
using std::tuple;
using std::vector;
using std::wregex;

using shared::service::unique_file_service;

using shared::service::make_unique_file_service;

namespace shared::file_service_tests
{

template <class PREDICATE>
tuple<unique_file_service, vector<path>> arrange(path const& folder, PREDICATE predicate);

TEST(file_service, returns_no_files_when_path_is_not_directory)
{
    // arrange
    auto const windows_directory = path(LR"(C:\windows\system32\cmd.exe)");
    wregex const filter(LR"(.*\.exe$)");
    auto const service = make_unique_file_service();

    // Act
    auto const files = service->get_files_from_directory(windows_directory, filter);

    // Assert
    ASSERT_EQ(0ULL, files.size());
}

TEST(file_service, returns_all_files_matching_filter)
{
    // arrange
    auto const windows_directory = path(LR"(C:\windows)");
    wregex const filter(LR"(.*\.exe$)");
    auto [service, expected] = arrange(windows_directory, 
        [&filter](directory_entry const& entry) {
           return regex_match(entry.path().wstring(), filter);
        });

    // Act
    auto const files = service->get_files_from_directory(windows_directory, filter);

    // Assert
    ASSERT_TRUE(equal(begin(expected), end(expected), begin(files)));
}



template <class PREDICATE>
tuple<unique_file_service, vector<path>> arrange(path const& folder, PREDICATE predicate)
{
    auto service = make_unique_file_service();
    return tuple<unique_file_service, vector<path>>(service.release(), tests::populate_expected_files(folder, predicate));
}

}
