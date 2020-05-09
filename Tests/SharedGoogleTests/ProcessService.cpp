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
#include <ProcessService.h>
#include <chrono>
#include "Common.h"

using std::chrono::duration;
using std::chrono::steady_clock;
using std::filesystem::path;
using std::make_unique;
using std::tie;
using std::tuple;
using std::unique_ptr;
using std::vector;

using Shared::Service::IProcessService;
using Shared::Service::ProcessService;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
using std::literals::chrono_literals::operator ""s;
#pragma warning(pop)

namespace Shared::ProcessServiceTests {

    template <class PREDICATE>
    tuple<unique_ptr<IProcessService>, vector<path>> Arrange(path const& folder, PREDICATE predicate);

    TEST(ProcessService, StartThrowsWhenFileNotFound) {
        unique_ptr<IProcessService> const service = make_unique<ProcessService>();

        auto const process = service->StartProcess(""s, ""s);

        ASSERT_FALSE(process.HasValue());
    }

    TEST(ProcessService, ReturnsProcessValueWhenFileFound) {
        auto const xcopyExe = R"(c:\windows\system32\xcopy.exe)"s;
        unique_ptr<IProcessService> const service = make_unique<ProcessService>();

        auto const process = service->StartProcess(xcopyExe, ""s);

        ASSERT_TRUE(process.HasValue());
        process->WaitForExit();
    }

    TEST(ProcessService, ExitCodeNonZeroWithBadCommand) {
        std::string const xcopyExe = R"(c:\windows\system32\xcopy.exe)"s;
        unique_ptr<IProcessService> const service = make_unique<ProcessService>();

        auto const process = service->StartProcess(xcopyExe, ""s);

        EXPECT_TRUE(process.HasValue());
        process->WaitForExit();

        auto const exitCode = process->ExitCode();
        EXPECT_TRUE(exitCode.has_value());

        ASSERT_NE(0, exitCode.value());
    }

#   ifdef _WIN64
    constexpr auto const CommandExe = R"(c:\windows\system32\cmd.exe)";
#   else
    constexpr auto const CommandExe = R"(c:\windows\SysWOW64\cmd.exe)";
#   endif

    TEST(ProcessService, ExitCodeZeroWithGoodCommand) {
        // Assert / Act
        unique_ptr<IProcessService> const service = make_unique<ProcessService>();
        auto const process = service->StartProcess(CommandExe, "/c echo \"Test\"");

        EXPECT_TRUE(process.HasValue());
        process->WaitForExit();

        auto const exitCode = process->ExitCode();
        EXPECT_TRUE(exitCode.has_value());

        // Assert
        ASSERT_EQ(0, exitCode.value());
    }

    TEST(ProcessService, WaitsForProcessToEnd) {
        auto const xcopyExe = R"(c:\windows\system32\xcopy.exe)"s;
        unique_ptr<IProcessService> const service = make_unique<ProcessService>();
        auto const start = steady_clock::now();

        auto const process = service->StartProcess(CommandExe, "/c Sleep 1");
        process->WaitForExit();

        auto const end = steady_clock::now();
        ASSERT_GE(duration<double>(end - start).count(), 1.0);
    }


    TEST(ProcessService, ProcessByNameFindsMatch) {
        // Assert / Act
        unique_ptr<IProcessService> const service = make_unique<ProcessService>();
        auto const process = service->StartProcess(CommandExe, "/c Sleep 1");
        auto const matchingProcesses = service->GetProcessesByName("cmd.exe");

        process->WaitForExit();

        // Assert
        ASSERT_GE(matchingProcesses.size(), 0UL);
    }
    TEST(ProcessService, NoProcessesFoundWithEmptyProcessname) {
        // Arrange
        unique_ptr<IProcessService> const service = make_unique<ProcessService>();
        // Act
        auto const matchingProcesses = service->GetProcessesByName(""s);
        // Assert
        ASSERT_EQ(matchingProcesses.size(), 0);
    }

    TEST(ProcessService, GetPathFromRunningPathReturnsPath) {
        // Arrange
        unique_ptr<IProcessService> const service = make_unique<ProcessService>();
        auto const runningProcess = service->StartProcess(CommandExe, "/c Sleep 1");

        // Act
        auto const path = service->GetPathToRunningProcess("cmd.exe");
        runningProcess->WaitForExit();

        // Assert
        ASSERT_TRUE(path.has_value());

    }

    TEST(ProcessService, GetPathFromRunningPathReturnsCorrectPath) {
        // Arrange
        std::filesystem::path expected(CommandExe);

        unique_ptr<IProcessService> const service = make_unique<ProcessService>();
        auto const runningProcess = service->StartProcess(CommandExe, "/c Sleep 1");

        // Act
        auto const path = service->GetPathToRunningProcess("cmd.exe");

        // Assert
        ASSERT_EQ(expected, path);
    }


    template <class PREDICATE>
    tuple<unique_ptr<IProcessService>, vector<path>> Arrange(path const& folder, PREDICATE predicate) {
        unique_ptr<IProcessService> service = make_unique<ProcessService>();
        return tuple<unique_ptr<IProcessService>, vector<path>>(service.release(), Tests::PopulateExpectedFiles(folder, predicate));
    }

}
