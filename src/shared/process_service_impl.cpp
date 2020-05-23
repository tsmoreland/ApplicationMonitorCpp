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
#include "process_service_impl.h"
#include "process_impl.h"

using std::back_inserter;
using std::copy_if;
using std::make_unique;
using std::move;
using std::optional;
using std::regex_match;
using std::string_view;
using std::transform;
using std::unique_ptr;
using std::vector;

using shared::model::process;
using shared::model::process_impl;
using shared::model::unique_process;

namespace shared::service
{

unique_process process_service_impl::start_process(string_view const& filename, string_view const& arguments) const noexcept
{
    try {
        return unique_process(process_impl::start(filename, arguments));
    }
    catch (const std::exception&) {
        return unique_process();
    }
}
vector<unique_process> process_service_impl::get_processes_by_name(string_view const& process_name) const noexcept
{
    try {
        const size_t minimumSize = 100;
        auto process_implementations = process_impl::get_processes_by_name(process_name);
        vector<unique_process> processes{};
        processes.reserve(minimumSize);

        transform(begin(process_implementations), end(process_implementations), back_inserter(processes),
            [](auto& pImpl) {
                return unique_process(move(pImpl));
            });

        return processes;
    }
    catch (std::exception const&) {
        return vector<unique_process>();
    }
}
optional<std::filesystem::path> process_service_impl::get_path_to_running_process(string_view const& process_name) const noexcept
{
    return process_impl().get_path_to_running_process(process_name);
}


}