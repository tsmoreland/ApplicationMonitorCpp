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
#include <tasks/task_base.h>

namespace tasks
{

bool task_base::is_done() const
{
    return m_current_state == task_status::COMPLETE;
}
task_status task_base::get_current_state() const noexcept
{
    return m_current_state;
}
optional_remaining_milliseconds task_base::get_estimated_time_remaining() const noexcept
{
    return m_time_remaining;
}

inline void task_base::update_task_status(task_status const value)
{
    m_current_state = value;
}

inline void task_base::update_time_remaining(optional_remaining_milliseconds const value)
{
    m_time_remaining = value;
}

}
