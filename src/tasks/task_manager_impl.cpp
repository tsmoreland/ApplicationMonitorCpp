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
#include "task_manager_impl.h"

namespace tasks
{

task_manager_impl::task_manager_impl(task_manager_impl&& other) noexcept
{
}
task_manager_impl& task_manager_impl::operator=(task_manager_impl&& other) noexcept
{
    return *this;
}

void task_manager_impl::start()
{
    if (is_running())
        throw std::domain_error("Already running");
}

void task_manager_impl::stop()
{
    if (!is_running())
        return;
}

bool task_manager_impl::is_running() const noexcept
{
    return static_cast<bool>(m_is_running);
}

void task_manager_impl::add(std::initializer_list<unique_task_base>&& tasks)
{
    std::lock_guard<std::mutex> guard(m_lock);
    std::move(begin(tasks), end(tasks), std::back_inserter(m_tasks));
}

}
