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

task_manager::task_manager()
    : m_pImpl(std::make_unique<tasks::task_manager_impl>())
{
}

task_manager::task_manager(task_manager&& other) noexcept
    : m_pImpl(other.m_pImpl.release())
{
}

task_manager::~task_manager()
{
    m_pImpl.reset();
}

void task_manager::start()
{
    if (m_pImpl == nullptr)
        return;

    m_pImpl->start();
}

void task_manager::stop()
{
    m_pImpl->stop();
}

bool task_manager::is_running() const noexcept
{
    return m_pImpl->is_running();
}

void task_manager::add(std::initializer_list<unique_task_base>&& tasks)
{
    if (m_pImpl == nullptr)
        return;
    //m_pImpl->add(tasks);
}

task_manager& task_manager::operator=(task_manager&& other) noexcept
{
    swap(other);
    return *this;
}

void task_manager::swap(task_manager& other) noexcept
{
    std::swap(m_pImpl, other.m_pImpl);
}

void swap(task_manager& left, task_manager& right) noexcept
{
    left.swap(right);
}

}
