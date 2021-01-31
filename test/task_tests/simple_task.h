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

#pragma once

#include "simple_task_base.h"

namespace task_test
{
    constexpr task_status get_initial_state()
    {
        return task_status::PENDING;
    }

    /// <summary>
    /// returns the expected transition this matches the result of get_next_state as is intended to verify
    /// that the correct transition occurred.
    /// </summary>
    /// <param name="current">initial state</param>
    /// <returns>expected new state resulting from get_next_state</returns>
    constexpr task_status get_expected_state_transition(task_status const current)
    {
        switch (current)
        {
        case task_status::PENDING:
            return task_status::READY;
        case task_status::READY:
            return task_status::RUNNING;
        case task_status::RUNNING:
            return task_status::COMPLETE;
        case task_status::COMPLETE:
            return task_status::FAILED;
        case task_status::FAILED:
        default:
            return task_status::PENDING;
        }
    }


    template<task_status STATUS>
    class simple_task_state final : public simple_task_state_base
    {
    public:
        /// <summary>
        /// transitions to the next state if ready
        /// </summary>
        /// <returns>the next state or this object if not ready for transition</returns>
        [[nodiscard]] unique_task_state_base get_next_state() override
        {
            switch (STATUS)
            {
            case task_status::PENDING:
                return std::make_unique<simple_task_state<task_status::READY>>();
            case task_status::READY:
                return std::make_unique<simple_task_state<task_status::RUNNING>>();
            case task_status::RUNNING:
                return std::make_unique<simple_task_state<task_status::COMPLETE>>();
            case task_status::COMPLETE:
                return std::make_unique<simple_task_state<task_status::FAILED>>();
            case task_status::FAILED:
            default:
                return std::make_unique<simple_task_state<task_status::PENDING>>();
            }
        }

        /// <summary>
        /// returns the estimated time remaining in milliseconds for current task if known
        /// </summary>
        /// <returns>optional time remaining in milliseconds</returns>
        [[nodiscard]] std::optional<std::chrono::milliseconds> get_time_remaining() const override
        {
            return std::optional<std::chrono::milliseconds>();
        }

        /// <summary>
        /// current task status of this object
        /// </summary>
        /// <returns>task status of this object</returns>
        [[nodiscard]] task_status get_task_status() const override
        {
            return STATUS;
        }

        /// <summary>
        /// returns true if other equals current object
        /// </summary>
        /// <param name="other">object to compare against</param>
        /// <returns>true if equal; otherwise, false</returns>
        [[nodiscard]] bool equals(simple_task_state_base const& other) const override
        {
            if (!simple_task_state_base::equals(other))
                return false;

            try {
                auto const& typed_other = dynamic_cast<simple_task_state<STATUS> const&>(other);

                // this where additional logic would go
                return true;

            } catch (std::bad_cast const&) {
                return false;
            }
        }
    };

    using simple_task = task<simple_task_state_base>;

    inline simple_task_state_base::unique_task_state_base simple_task_state_base::initialize()
    {
        static_assert(get_initial_state() == task_status::PENDING, "initialize should status should match get_initial_state");
        return std::make_unique<simple_task_state<task_status::PENDING>>();
    }

}
