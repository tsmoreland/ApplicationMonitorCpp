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

#define BOOST_TEST_MODULE simple_task_test
#include <boost/test/included/unit_test.hpp>

using namespace task_test;

using collection::contains;
using std::vector;

#pragma warning(push)
#pragma warning(disable : 4455)
using std::string_literals::operator ""s;
#pragma warning(pop)

BOOST_AUTO_TEST_SUITE(simple_task_test)

/// <summary>
/// verify the initial state returns the expected value
/// </summary>
BOOST_AUTO_TEST_CASE(verify_initial_state)
{
    // Arrange + Act
    simple_task const task{};

    // Assert
    BOOST_ASSERT_MSG(task.get_current_state() == task_status::PENDING, "Unexpected state");
}

/// <summary>
/// Verify that all transitions can be hit using process, more of a test of simple_task than task but verifying
/// this may be useful in subsequent tests
/// </summary>
BOOST_AUTO_TEST_CASE(process_transitions_to_expected_state)
{
    // Arrange
    // Arrange + Act
    simple_task task{};
    auto const initial_state = task.get_current_state();
    vector<task_status> const expected_status {
        task_status::PENDING,
        task_status::READY, 
        task_status::RUNNING,
        task_status::COMPLETE,
        task_status::FAILED
    };
    vector<task_status> actual_state{};

    // extending a return to PENDING within simple_task
    do {
        auto const current_state = task.get_current_state();

        // Act
        task.process();
        auto const new_state = task.get_current_state();
        auto const expected_state = get_expected_state_transition(current_state);

        // Assert
        BOOST_ASSERT_MSG(expected_state == new_state, 
            ("unexpected state from:"s + tasks::to_string(current_state) + 
                " to:"s + tasks::to_string(new_state) + 
                " expected:"s + tasks::to_string(expected_state)).c_str());
        actual_state.push_back(new_state);
        
    } while (task.get_current_state() != initial_state);

    // Further Assert
    BOOST_ASSERT_MSG(std::all_of(cbegin(expected_status), cend(expected_status), 
        [const_actual = static_cast<vector<task_status> const&>(actual_state)](auto const status) {
            return contains(const_actual, status);
        }), 
        "Missing expected state");
}

BOOST_AUTO_TEST_SUITE_END()