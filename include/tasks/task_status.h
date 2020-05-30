//
// Copyright � 2020 Terry Moreland
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

#include <string>

namespace tasks
{
    /// <summary>Current task_status, used to task state machine</summary>
    enum class task_status
    {
        /// <summary>initializing or not yet ready to run</summary>
        PENDING,
        /// <summary>Ready for execution but hasn't been started</summary>
        READY,
        /// <summary>Currently running</summary>
        RUNNING,
        /// <summary>Completed with success</summary>
        COMPLETE,
        /// <summary>Completed with error</summary>
        FAILED,
    }; 

    inline std::string to_string(task_status const status)
    {
        switch (status)
        {
        case task_status::PENDING:
            return "Pending";
        case task_status::READY:
            return "Ready";
        case task_status::RUNNING:
            return "Running";
        case task_status::COMPLETE:
            return "Complete";
        case task_status::FAILED:
            return "Failed";
        default:
            return "Unknown (to_string needs updated)";
        }
    }

}
