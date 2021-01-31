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

#include <tasks/task_manager.h>
#include <vector>
#include <atomic>
#include <mutex>

namespace tasks
{
    class task_manager_impl final
    {
    public:
        explicit task_manager_impl() = default;
        task_manager_impl(task_manager_impl const&) = delete;
        task_manager_impl(task_manager_impl&& other) noexcept;
        ~task_manager_impl() = default;

        void start();
        void stop();
        [[nodiscard]] bool is_running() const noexcept;
        void add(std::initializer_list<unique_task_base>&& tasks);

        task_manager_impl& operator=(task_manager_impl const&) = delete;
        task_manager_impl& operator=(task_manager_impl&& other) noexcept;
    private:
        std::mutex m_lock{};
        std::atomic<bool> m_is_running{false};
        std::vector<unique_task_base> m_tasks{};
    };
}
