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

#include <chrono>
#include <memory>
#include <optional>
#include <typeinfo>
#include <typeindex>
#include <tasks/task.h>

namespace task_test
{
    using tasks::task;
    using tasks::task_status;

    class simple_task_state_base
    {
    public:
        using unique_task_state_base = std::unique_ptr<simple_task_state_base>;

        simple_task_state_base() = default;
        simple_task_state_base(simple_task_state_base&&) noexcept = default;        
        simple_task_state_base(simple_task_state_base const&) = default;
        virtual ~simple_task_state_base() = default;

        static unique_task_state_base initialize();

        /// <summary>
        /// transitions to the next state if ready
        /// </summary>
        /// <returns>the next state or this object if not ready for transition</returns>
        [[nodiscard]] virtual unique_task_state_base get_next_state() = 0;

        /// <summary>
        /// returns the estimated time remaining in milliseconds for current task if known
        /// </summary>
        /// <returns>optional time remaining in milliseconds</returns>
        [[nodiscard]] virtual std::optional<std::chrono::milliseconds> get_time_remaining() const = 0;

        /// <summary>
        /// current task status of this object
        /// </summary>
        /// <returns>task status of this object</returns>
        [[nodiscard]] virtual tasks::task_status get_task_status() const = 0;

        /// <summary>
        /// returns true if other equals current object
        /// </summary>
        /// <param name="other">object to compare against</param>
        /// <returns>true if equal; otherwise, false</returns>
        [[nodiscard]] virtual bool equals(simple_task_state_base const& other) const
        {
            if (this == &other)
                return true;
            if (get_task_status() != other.get_task_status())
                return false;

            auto const& this_type_id = typeid(*this);
            auto const& other_type_id = typeid(other);

            return (std::type_index(this_type_id) != std::type_index(other_type_id));
        }

        simple_task_state_base& operator=(simple_task_state_base&&) noexcept = default;
        simple_task_state_base& operator=(simple_task_state_base const&) = default;

        friend bool operator==(simple_task_state_base const& left, simple_task_state_base const& right);
        friend bool operator!=(simple_task_state_base const& left, simple_task_state_base const& right);

        [[nodiscard]] bool operator==(simple_task_state_base const& other) const
        {
            return equals(other);
        }
        [[nodiscard]] bool operator!=(simple_task_state_base const& other) const
        {
            return !equals(other);
        }
    };
    [[nodiscard]] inline bool operator==(simple_task_state_base const& left, simple_task_state_base const& right)
    {
        return left.equals(right);
    }
    [[nodiscard]] inline bool operator!=(simple_task_state_base const& left, simple_task_state_base const& right)
    {
        return !left.equals(right);
    }

}
