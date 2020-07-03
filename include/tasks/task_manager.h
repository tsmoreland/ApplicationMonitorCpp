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

#include <memory>
#include <tasks/task.h>

namespace tasks
{
    class task_manager_impl;

    class task_manager final
    {
    public:
        TASKS_DLL explicit task_manager();
        task_manager(task_manager const&) = delete;
        TASKS_DLL task_manager(task_manager&& other) noexcept;
        TASKS_DLL ~task_manager();

        TASKS_DLL void start();
        TASKS_DLL void stop();
        TASKS_DLL [[nodiscard]] bool is_running() const noexcept;
        TASKS_DLL void add(std::initializer_list<unique_task_base>&& tasks);

        template <typename... TASKS>
        void add(TASKS&&... args)
        {
            add(std::initializer_list<unique_task_base>{std::forward<unique_task_base>(args)...});
        }

        task_manager& operator=(task_manager const&) = delete;
        TASKS_DLL task_manager& operator=(task_manager&& other) noexcept;
        TASKS_DLL void swap(task_manager& other) noexcept;

    private:
        std::unique_ptr<task_manager_impl> m_pImpl;
    };

    TASKS_DLL void swap(task_manager& left, task_manager& right) noexcept;

}
