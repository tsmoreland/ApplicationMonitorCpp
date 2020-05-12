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
#include <process_service_impl.h>
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

using shared::service::process_service;
using shared::service::process_service_impl;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
using std::literals::chrono_literals::operator ""s;
#pragma warning(pop)

namespace Shared::ProcessServiceTests
{

template <class PREDICATE>
tuple<unique_ptr<process_service>, vector<path>> Arrange(path const& folder, PREDICATE predicate);

TEST(process_service, start_throws_when_file_not_found)
{
    unique_ptr<process_service> const service = make_unique<process_service_impl>();

    auto const process = service->start_process(""s, ""s);

    ASSERT_EQ(process, nullptr);
}

TEST(ProcessService, returns_process_value_when_file_found)
{
    auto const xcopyExe = R"(c:\windows\system32\xcopy.exe)"s;
    unique_ptr<process_service> const service = make_unique<process_service_impl>();

    auto const process = service->start_process(xcopyExe, ""s);

    ASSERT_NE(process, nullptr);
    process->wait_for_exit();
}

TEST(process_service, exit_code_non_zero_with_bad_command)
{
    auto const xcopyExe = R"(c:\windows\system32\xcopy.exe)"s;
    unique_ptr<process_service> const service = make_unique<process_service_impl>();

    auto const process = service->start_process(xcopyExe, ""s);

    EXPECT_TRUE(process != nullptr);
    process->wait_for_exit();

    auto const exitCode = process->exit_code();
    EXPECT_TRUE(exitCode.has_value());

    ASSERT_NE(0, exitCode.value());
}

#   ifdef _WIN64
constexpr auto const CommandExe = R"(c:\windows\system32\cmd.exe)";
#   else
constexpr auto const CommandExe = R"(c:\windows\SysWOW64\cmd.exe)";
#   endif

TEST(process_service, exit_code_zero_with_good_command)
{
    // Assert / Act
    unique_ptr<process_service> const service = make_unique<process_service_impl>();
    auto const process = service->start_process(CommandExe, "/c echo \"Test\"");

    EXPECT_TRUE(process != nullptr);
    process->wait_for_exit();

    auto const exitCode = process->exit_code();
    EXPECT_TRUE(exitCode.has_value());

    // Assert
    ASSERT_EQ(0, exitCode.value());
}

TEST(process_service, waits_for_process_to_end)
{
    auto const xcopyExe = R"(c:\windows\system32\xcopy.exe)"s;
    unique_ptr<process_service> const service = make_unique<process_service_impl>();
    auto const start = steady_clock::now();

    auto const process = service->start_process(CommandExe, "/c Sleep 1");
    process->wait_for_exit();

    auto const end = steady_clock::now();
    ASSERT_GE(duration<double>(end - start).count(), 1.0);
}


TEST(process_service, ProcessByNameFindsMatch)
{
    // Assert / Act
    unique_ptr<process_service> const service = make_unique<process_service_impl>();
    auto const process = service->start_process(CommandExe, "/c Sleep 1");
    auto const matchingProcesses = service->get_processes_by_name("cmd.exe");

    process->wait_for_exit();

    // Assert
    ASSERT_GE(matchingProcesses.size(), 0UL);
}
TEST(process_service, no_processes_found_with_empty_process_name)
{
    // Arrange
    unique_ptr<process_service> const service = make_unique<process_service_impl>();
    // Act
    auto const matchingProcesses = service->get_processes_by_name(""s);
    // Assert
    ASSERT_EQ(matchingProcesses.size(), 0);
}

TEST(process_service, get_path_from_running_path_returns_path)
{
    // Arrange
    unique_ptr<process_service> const service = make_unique<process_service_impl>();
    auto const runningProcess = service->start_process(CommandExe, "/c Sleep 2");

    // Act
    auto const path = service->get_path_to_running_process("cmd.exe");
    runningProcess->wait_for_exit();

    // Assert
    ASSERT_TRUE(path.has_value());

}

TEST(process_service, get_path_from_running_path_returns_correct_path)
{
    // Arrange
    std::filesystem::path expected(CommandExe);

    unique_ptr<process_service> const service = make_unique<process_service_impl>();
    auto const runningProcess = service->start_process(CommandExe, "/c Sleep 2");

    // Act
    auto const path = service->get_path_to_running_process("cmd.exe");

    // Assert
    ASSERT_EQ(expected, path);
}


template <class PREDICATE>
tuple<unique_ptr<process_service>, vector<path>> Arrange(path const& folder, PREDICATE predicate)
{
    unique_ptr<process_service> service = make_unique<process_service_impl>();
    return tuple<unique_ptr<process_service>, vector<path>>(service.release(), Tests::PopulateExpectedFiles(folder, predicate));
}

}
