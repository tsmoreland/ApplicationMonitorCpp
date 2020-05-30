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
#include <tasks/task_status.h>
#include <tasks/tasks_export.h>
#include <optional>

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

        [[nodiscard]] TASKS_DLL bool is_complete() const;
        [[nodiscard]] TASKS_DLL task_status get_current_state() const noexcept;
        [[nodiscard]] TASKS_DLL optional_remaining_milliseconds get_estimated_time_remaining() const noexcept;

    protected:
        TASKS_DLL explicit task_base() = default;

        TASKS_DLL void update_task_status(task_status const value);
        TASKS_DLL void update_time_remaining(optional_remaining_milliseconds const value);

    private:
        task_status m_current_state{task_status::PENDING};
        optional_remaining_milliseconds m_time_remaining{};
    };
    
    template <typename TASK>
    concept Task = std::is_base_of<task_base, TASK>::value;

}
