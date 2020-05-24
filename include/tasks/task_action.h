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
#include <future>
#include <tasks/task_state.h>

namespace tasks
{
    /// <summary>worker method for task, represents a current state and provides process used to transition to the next state (if ready)</summary>
    class task_action
    {
        task_action(task_action const&) = default;
        task_action(task_action&&) noexcept = default;
        task_action& operator=(task_action const&) = default;
        task_action& operator=(task_action&&) noexcept = default;

        virtual ~task_action() = default;
        virtual std::future<std::pair<task_state, std::chrono::milliseconds>> process_async() = 0;
    };

    template<typename TASK_ACTION>
    concept TaskAction = requires(TASK_ACTION a) {
        requires std::is_same<std::future<std::pair<task_state, std::chrono::milliseconds>>, decltype(std::declval<TASK_ACTION>().process_async())>::value;
    };

    
}
