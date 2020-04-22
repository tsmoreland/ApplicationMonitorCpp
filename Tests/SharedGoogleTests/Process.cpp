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

#include <chrono>

using std::chrono::duration;
using std::invalid_argument;
using std::string;
using std::unique_ptr;

#include "Process.h"
using Shared::Model::IProcess;
using Shared::Model::Process;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
using std::literals::chrono_literals::operator ""s;
#pragma warning(pop)

namespace Shared::Tests
{
    TEST(Process, StartThrowsWhenFileNotFound)
    {
        ASSERT_THROW(auto const process = Process::Start(""s, ""s), invalid_argument);
    }

    std::string const XcopyExe = R"(c:\windows\system32\xcopy.exe)"s;
    TEST(Process, ExitCodeHasExitCodeForBadCommand)
    {
        // Assert / Act
        auto const process = Process::Start(XcopyExe, ""s);
        process->WaitForExit();
        auto const exitCode = process->ExitCode();

        ASSERT_TRUE(exitCode.has_value());
    }

    TEST(Process, ExitCodeNonZeroWithBadCommand)
    {
        // Assert / Act
        auto const process = Process::Start(XcopyExe, ""s);
        process->WaitForExit();
        auto const exitCode = process->ExitCode();

        // Assert
        ASSERT_NE(0, exitCode.value());
    }

    std::string const CommandExe = R"(c:\windows\system32\cmd.exe)"s;
    TEST(Process, ExitCodeZeroWithGoodCommand)
    {
        // Assert / Act
        auto const process = Process::Start(CommandExe, R"(/c echo ""Test"")"s);
        process->WaitForExit();
        auto const exitCode = process->ExitCode();

        // Assert
        ASSERT_EQ(0, exitCode.value());
    }

    TEST(Process, ProcessByNameFindsMatch)
    {
        // Assert / Act
        auto const process = Process::Start(CommandExe, R"(/c Sleep 1)"s);
        auto const matchingProcesses = Process::GetProcessesByName("cmd.exe");

        process->WaitForExit();
        static_cast<void>(process->ExitCode());

        // Assert
        ASSERT_GE(matchingProcesses.size(), 0);
    }
    TEST(Process, NoProcessesFoundWithEmptyProcessname)
    {
        // Assert / Act
        auto const matchingProcesses = Process::GetProcessesByName(""s);
        ASSERT_EQ(matchingProcesses.size(), 0);
    }

    TEST(Process, GetPathFromRunningPathReturnsPath)
    {
        // Arrange
        auto const runningProcess = Process::Start(CommandExe, R"(/c Sleep 1)"s);
        Process const process{};

        // Act
        auto const path = process.GetPathToRunningProcess("cmd.exe");
        runningProcess->WaitForExit();

        // Assert
        ASSERT_TRUE(path.has_value());

    }

    TEST(Process, GetPathFromRunningPathReturnsCorrectPath)
    {
        // Arrange
        std::filesystem::path expected(CommandExe);
        auto const runningProcess = Process::Start(CommandExe, R"(/c Sleep 1)"s);
        Process const process{};

        // Act
        auto const path = process.GetPathToRunningProcess("cmd.exe");

        // Assert
        ASSERT_EQ(expected, path);
    }

}