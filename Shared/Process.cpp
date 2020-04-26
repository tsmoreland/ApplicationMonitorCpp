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
#include "Process.h"
#include "ProcessImpl.h"

using ProcessImpl = Shared::Model::ProcessImpl;

using std::filesystem::path;
using std::make_unique;
using std::move;
using std::nullopt;
using std::optional;
using std::string;
using std::string_view;
using std::unique_ptr;
using std::vector;

namespace Shared::Model {

    unique_ptr<IProcess> Process::Start(string_view const& filename, string_view const& arguments) {
        unique_ptr<Process> process{};
        process.reset(new Process(ProcessImpl::Start(filename, arguments).release()));
        return process;
    }
    vector<unique_ptr<IProcess>> Process::GetProcessesByName(std::string_view const& processName) {
        const size_t minimumSize = 100;
        auto processImplementations = ProcessImpl::GetProcessesByName(processName);
        vector<unique_ptr<IProcess>> processes{};
        processes.reserve(minimumSize);

        for (auto& pImpl : processImplementations)
            processes.emplace_back(new Process(pImpl.release()));
        return processes;
    }

    Process::Process() {
        pImpl = make_unique<ProcessImpl>();
    }

    // destructor only needed because we're exporting and default would need to know more about pImpl
    Process::~Process() {
        pImpl.reset();
    }

    unsigned long Process::GetId() const noexcept {
        return pImpl->GetId();
    }

    bool Process::IsRunning() const noexcept {
        return pImpl->IsRunning();
    }

    optional<DWORD> Process::ExitCode() const noexcept {
        return pImpl->ExitCode();
    }

    void Process::WaitForExit() const noexcept {
        pImpl-> WaitForExit();
    }

    optional<path> Process::GetPathToRunningProcess(string_view const& processName) const noexcept {
        try {
            return ProcessImpl::GetPathToRunningProcess(processName);
        }
        catch (std::exception&) {
            return nullopt;
        }
    }

    Process::Process(ProcessImpl* pImpl)
        : IProcess()
        , pImpl{pImpl} {
    }
    Process& Process::operator=(Process&& other) noexcept {
        pImpl = move(other.pImpl);
        return *this;
    }
}