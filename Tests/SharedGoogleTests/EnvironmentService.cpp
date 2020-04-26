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
using std::regex_match;
using std::string;
using std::string_view;
using std::tie;
using std::tuple;
using std::unique_ptr;
using std::vector;
using std::wregex;
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

    TEST(EnvironmentService, ReturnsNoFilesWhenPathIsNotDirectory) {
        // Arrange
        auto const windowsDirectory = FileSystem::path(LR"(C:\windows\system32\cmd.exe)");
        wregex const filter(LR"(.*\.exe$)");
        unique_ptr<IEnvironmentService> const service = make_unique<EnvironmentService>();

        // Act
        auto const files = service->GetFilesFromDirectory(windowsDirectory, filter);

        // Assert
        ASSERT_EQ(0ULL, files.size());
    }

    TEST(EnvironmentService, ReturnsAllFilesMatchingFilter) {
        // Arrange
        auto const windowsDirectory = FileSystem::path(LR"(C:\windows)");
        unique_ptr<IEnvironmentService> service;
        vector<FileSystem::path> expected{};
        wregex const filter(LR"(.*\.exe$)");
        tie(service, expected) = Arrange(windowsDirectory, 
            [&filter](FileSystem::directory_entry const& entry) {
               return regex_match(entry.path().wstring(), filter);
            });

        // Act
        auto const files = service->GetFilesFromDirectory(windowsDirectory, filter);

        // Assert
        ASSERT_TRUE(equal(begin(expected), end(expected), begin(files)));
    }

    TEST(EnvironmentService, StartThrowsWhenFileNotFound) {
        unique_ptr<IEnvironmentService> const service = make_unique<EnvironmentService>();

        auto const process = service->StartProcess(""s, ""s);

        ASSERT_FALSE(process.has_value());
    }

    TEST(EnvironmentService, ReturnsProcessValueWhenFileFound) {
        std::string const XcopyExe = R"(c:\windows\system32\xcopy.exe)"s;
        unique_ptr<IEnvironmentService> const service = make_unique<EnvironmentService>();

        auto const process = service->StartProcess(XcopyExe, ""s);

        ASSERT_TRUE(process.has_value());
        process.value()->WaitForExit();
    }

    TEST(EnvironmentService, ExitCodeNonZeroWithBadCommand) {
        std::string const XcopyExe = R"(c:\windows\system32\xcopy.exe)"s;
        unique_ptr<IEnvironmentService> const service = make_unique<EnvironmentService>();

        auto const process = service->StartProcess(XcopyExe, ""s);

        EXPECT_TRUE(process.has_value());
        process.value()->WaitForExit();

        auto const exitCode = process.value()->ExitCode();
        EXPECT_TRUE(exitCode.has_value());

        ASSERT_NE(0, exitCode.value());
    }

    std::string const CommandExe = R"(c:\windows\system32\cmd.exe)"s;
    TEST(EnvironmentService, ExitCodeZeroWithGoodCommand) {
        // Assert / Act
        unique_ptr<IEnvironmentService> const service = make_unique<EnvironmentService>();
        auto const process = service->StartProcess(CommandExe, "/c echo \"Test\"");

        EXPECT_TRUE(process.has_value());
        process.value()->WaitForExit();

        auto const exitCode = process.value()->ExitCode();
        EXPECT_TRUE(exitCode.has_value());

        // Assert
        ASSERT_EQ(0, exitCode.value());
    }

    TEST(EnvironmentService, ProcessByNameFindsMatch) {
        // Assert / Act
        unique_ptr<IEnvironmentService> const service = make_unique<EnvironmentService>();
        auto const process = service->StartProcess(CommandExe, "/c Sleep 1");
        auto const matchingProcesses = service->GetProcessesByName("cmd.exe");

        process.value()->WaitForExit();

        // Assert
        ASSERT_GE(matchingProcesses.size(), 0);
    }
    TEST(EnvironmentService, NoProcessesFoundWithEmptyProcessname) {
        // Arrange
        unique_ptr<IEnvironmentService> const service = make_unique<EnvironmentService>();
        // Act
        auto const matchingProcesses = service->GetProcessesByName(""s);
        // Assert
        ASSERT_EQ(matchingProcesses.size(), 0);
    }

    TEST(EnvironmentService, GetPathFromRunningPathReturnsPath) {
        // Arrange
        unique_ptr<IEnvironmentService> const service = make_unique<EnvironmentService>();
        auto const runningProcess = service->StartProcess(CommandExe, "/c Sleep 1");

        // Act
        auto const path = service->GetPathToRunningProcess("cmd.exe");
        runningProcess.value()->WaitForExit();

        // Assert
        ASSERT_TRUE(path.has_value());

    }

    TEST(EnvironmentService, GetPathFromRunningPathReturnsCorrectPath) {
        // Arrange
        std::filesystem::path expected(CommandExe);
        unique_ptr<IEnvironmentService> const service = make_unique<EnvironmentService>();
        auto const runningProcess = service->StartProcess(CommandExe, "/c Sleep 1");

        // Act
        auto const path = service->GetPathToRunningProcess("cmd.exe");

        // Assert
        ASSERT_EQ(expected, path);
    }

    template <class PREDICATE>
    tuple<unique_ptr<IEnvironmentService>, vector<FileSystem::path>> Arrange(FileSystem::path const& folder, PREDICATE predicate) {
        unique_ptr<IEnvironmentService> service = make_unique<EnvironmentService>();

        vector<FileSystem::path> expected{};
        if (FileSystem::exists(folder) && FileSystem::is_directory(folder)) {
            auto const expectedFiles = FileSystem::directory_iterator(folder);
            copy_if(begin(expectedFiles), end(expectedFiles), back_inserter(expected), 
                [predicate](FileSystem::directory_entry const& entry) {
                    return entry.is_regular_file() && predicate(entry);
                });
        }

        return tuple<unique_ptr<IEnvironmentService>, vector<FileSystem::path>>(service.release(), expected);
    }
}

