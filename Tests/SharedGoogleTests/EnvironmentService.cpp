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
#include "EnvironmentService.h"

#include <chrono>

using std::chrono::duration;
using std::copy;
using std::copy_if;
using std::equal;
using std::invalid_argument;
using std::make_tuple;
using std::make_unique;
using std::move;
using std::string;
using std::string_view;
using std::tie;
using std::tuple;
using std::unique_ptr;
using std::vector;
using std::wstring;
using std::wstring_view;

using Shared::Services::IEnvironmentService;
using Shared::Services::EnvironmentService;

namespace FileSystem = std::filesystem;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
using std::literals::chrono_literals::operator ""s;
#pragma warning(pop)

namespace Shared::Tests
{
    template <class PREDICATE>
    tuple<unique_ptr<IEnvironmentService>, vector<FileSystem::path>> Arrange(FileSystem::path const& folder, PREDICATE predicate);

    TEST(EnvironmentService, ReturnsAllFilesWhenNoFilter)
    {
        // Arrange
        auto const windowsDirectory = FileSystem::path(LR"(C:\windows)");
        unique_ptr<IEnvironmentService> service;
        vector<FileSystem::path> expected{};
        auto const filter = L""s;
        tie(service, expected) = Arrange(windowsDirectory, [](FileSystem::directory_entry const& entry) { return true; });

        // Act
        auto const files = service->GetFilesFromDirectory(windowsDirectory, filter);

        // Assert
        ASSERT_TRUE(equal(begin(expected), end(expected), begin(files)));
    }

    TEST(EnvironmentService, ReturnsAllFilesMatchingFilterAtEnd)
    {
        // Arrange
        auto const windowsDirectory = FileSystem::path(LR"(C:\windows)");
        unique_ptr<IEnvironmentService> service;
        vector<FileSystem::path> expected{};
        auto const filter = L"*.exe"s;
        tie(service, expected) = Arrange(windowsDirectory, 
            [](FileSystem::directory_entry const& entry)
            {
               auto const path = entry.path().generic_string();
               return path.ends_with(".exe");
            });

        // Act
        auto const files = service->GetFilesFromDirectory(windowsDirectory, filter);

        // Assert
        ASSERT_TRUE(equal(begin(expected), end(expected), begin(files)));
    }
    TEST(EnvironmentService, ReturnsAllFilesMatchingFilterAtStart)
    {
        // Arrange
        auto const system32Directory = FileSystem::path(LR"(C:\windows\system32)");
        unique_ptr<IEnvironmentService> service;
        vector<FileSystem::path> expected{};
        auto const filter = L"Search.*"s;
        tie(service, expected) = Arrange(system32Directory, 
            [](FileSystem::directory_entry const& entry)
            {
               auto const path = entry.path().generic_string();
               return path.starts_with(".exe");
            });

        // Act
        auto const files = service->GetFilesFromDirectory(system32Directory, filter);

        // Assert
        ASSERT_TRUE(equal(begin(expected), end(expected), begin(files)));
    }

    template <class PREDICATE>
    tuple<unique_ptr<IEnvironmentService>, vector<FileSystem::path>> Arrange(FileSystem::path const& folder, PREDICATE predicate)
    {
        unique_ptr<IEnvironmentService> service = make_unique<EnvironmentService>();

        vector<FileSystem::path> expected{};
        auto const expectedFiles = FileSystem::directory_iterator(folder);
        copy_if(begin(expectedFiles), end(expectedFiles), back_inserter(expected), [predicate](FileSystem::directory_entry const& entry)
        {
            return entry.is_regular_file() && predicate(entry);
        });

        return tuple<unique_ptr<IEnvironmentService>, vector<FileSystem::path>>(service.release(), expected);
    }
}

