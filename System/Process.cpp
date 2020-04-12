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

using ProcessImpl = System::Implementation::ProcessImpl;
using std::unique_ptr;
using std::make_unique;
using std::optional;
using std::nullopt;

namespace System
{

    std::unique_ptr<Process> Process::Start(std::string const& filename, std::string const& arguments)
    {
        unique_ptr<Process> process{};
        auto pImpl = make_unique<ProcessImpl>(filename, arguments);
        process.reset(new Process(pImpl.release()));

        return process;
    }

    optional<DWORD> Process::GetId() const noexcept
    {
        return _pImpl ? _pImpl->GetId() : nullopt;
    }

    bool Process::IsRunning() const noexcept
    {
        return _pImpl->IsRunning();
    }

    optional<DWORD> Process::ExitCode() const noexcept
    {
        return _pImpl->ExitCode();
    }

    void Process::WaitForExit() const noexcept
    {
        _pImpl-> WaitForExit();
    }

    Process::Process(ProcessImpl* pImpl)
        : _pImpl{pImpl}
    {
        
    }


}