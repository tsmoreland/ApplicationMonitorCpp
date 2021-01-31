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
#include <chrono>
#include <exception>
#include <thread>

#include <tasks/task_manager.h>
#include <tasks/task_base.h>
#include "simple_task.h"

using tasks::task_manager;
using tasks::task;
using tasks::task_status;
using tasks::make_unique_task;
using task_test::simple_task;
using task_test::simple_task_state;
using task_test::simple_task_state_base;

template <typename EXPR>
void assert(EXPR const&expr) {
    _ASSERTE(expr);
}

void take_manager_workflow()
{
    /* what we're aiming for, maybe */
    task_manager manager{};

    manager.start();
    try {
        manager.start();

    } catch (std::domain_error const&) {
        // if called twice domain error thrown, cannot start when running
    }

    assert(manager.is_running()); // returns true when running

    //manager.add<example_task>(1, 2, "argument"); // example_task is the spec

    manager.add(
        make_unique_task<simple_task_state_base>(),
        make_unique_task<simple_task_state_base>(),
        make_unique_task<simple_task_state_base>()
    );

    manager.add({ make_unique_task<simple_task_state_base>(), make_unique_task<simple_task_state_base>() });

    std::this_thread::sleep_for(std::chrono::seconds(5));
    /*

    manager.stop(); // this should be optional, if not called
    */
}

/*
#include <ppl.h>
#include <ppltasks.h>

using Concurrency::task_group;
using Concurrency::task;
using Concurrency::make_task;

void with_ppl()
{
    auto t = make_task([]() { });

    //auto task_of_int = Concurrency::create_task([]() -> int { return 1; });
    auto task_of_int = Concurrency::make_task([]() { return 1; });

    task_of_int();

    task_group group{};
    group.run(task_of_int);
}
*/


