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
#include <tasks/tasks_export.h>
#include <tasks/task_status.h>

namespace tasks
{
    using optional_remaining_milliseconds = std::optional<std::chrono::milliseconds>;

    /// <summary>Represents a repeatble asynchronous operation with state that determines whether the operation can be run</summary>
    /// <remarks>not intended for direct use but serving as a base class and basis for a task_base concept</remarks>s
    class task_base  
    {
    public:
        TASKS_DLL task_base(task_base const&)  = delete;
        TASKS_DLL task_base(task_base&&) noexcept = default;
        TASKS_DLL virtual ~task_base() = default;

        TASKS_DLL task_base& operator=(task_base const&) = delete;
        TASKS_DLL task_base& operator=(task_base&&) noexcept = default;

        /// <summary>
        /// process current state
        /// </summary>
        virtual void process() = 0;

        /// <summary>
        /// returns true if task is complete or failed 
        /// </summary>
        [[nodiscard]] TASKS_DLL bool is_done() const;
        /// <summary>
        /// returns the current task_status value
        /// </summary>
        [[nodiscard]] TASKS_DLL task_status get_current_state() const noexcept;
        /// <summary>
        /// returns an optional time in milliseconds representing the estimated time remaining in milliseconds for the task in its current state
        /// </summary>
        /// <remarks>the time is provided is only meaningful for the current state, it represents how long before the next state is reached</remarks>
        [[nodiscard]] TASKS_DLL optional_remaining_milliseconds get_estimated_time_remaining() const noexcept;

    protected:
        TASKS_DLL explicit task_base() = default;

        TASKS_DLL void update_task_status(task_status const value);
        TASKS_DLL void update_time_remaining(optional_remaining_milliseconds const value);

    private:
        task_status m_current_state{task_status::PENDING};
        optional_remaining_milliseconds m_time_remaining{};
    };

    using unique_task_base = std::unique_ptr<task_base>;


}
