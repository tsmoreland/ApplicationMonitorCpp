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
#include "IEnvironmentService.h"
#include "EnvironmentService.h"
#include "Process.h"

using std::nullopt;
using std::optional;
using std::string_view;
using std::unique_ptr;
using std::vector;

using Shared::Domain::IProcess;
using Shared::Domain::Process;

namespace Shared::Services
{
    optional<std::unique_ptr<Shared::Domain::IProcess>> EnvironmentService::StartProcess(std::string_view const& filename, std::string_view const& arguments) const noexcept
    {
        try
        {
            return optional<unique_ptr<IProcess>>(Process::Start(filename, arguments));
        }
        catch (const std::exception&)
        {
            return nullopt;
        }
    }

    vector<std::unique_ptr<Shared::Domain::IProcess>> EnvironmentService::GetProcessesByName(std::string_view const& processName) const noexcept
    {
        try
        {
            return Process::GetProcessesByName(processName);
        }
        catch (const std::exception&)
        {
            return vector<unique_ptr<IProcess>>();
        }
    }
}
