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
#include "ProcessService.h"
#include "Process.h"

namespace filesystem = std::filesystem;

using std::back_inserter;
using std::copy_if;
using std::make_tuple;
using std::move;
using std::optional;
using std::regex_match;
using std::string_view;
using std::transform;
using std::unique_ptr;
using std::vector;

using Shared::Infrastructure::UniqueOwner;
using Shared::Model::IProcess;
using Shared::Model::Process;
using UniqueProcess = Shared::Infrastructure::UniqueOwner<Shared::Model::IProcess>;

namespace Shared::Service {

    UniqueProcess ProcessService::StartProcess(string_view const& filename, string_view const& arguments) const noexcept
    {
        try {
            return UniqueProcess(Process::Start(filename, arguments));
        }
        catch (const std::exception&) {
            return UniqueProcess::Empty();
        }
    }
    vector<UniqueProcess> ProcessService::GetProcessesByName(string_view const& processName) const noexcept {
        try {
            const size_t minimumSize = 100;
            auto processImplementations = Process::GetProcessesByName(processName);
            vector<UniqueProcess> processes{};
            processes.reserve(minimumSize);

            transform(begin(processImplementations), end(processImplementations), back_inserter(processes),
                [](auto& pImpl) {
                    return UniqueOwner<IProcess>(move(pImpl));
                });

            return processes;
        }
        catch (std::exception const&) {
            return vector<UniqueProcess>();
        }
    }
    optional<filesystem::path> ProcessService::GetPathToRunningProcess(string_view const& processName) const noexcept {
        return Process().GetPathToRunningProcess(processName);
    }


}