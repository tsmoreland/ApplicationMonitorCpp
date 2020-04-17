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
#include <tlhelp32.h>
#include "ProcessIterable.h"

using std::nullopt;
using std::optional;

namespace Shared::Infrastructure
{
    ProcessIterable::ProcessIterator::reference ProcessIterable::ProcessIterator::operator*() 
    {
        return _entry;
    }
    ProcessIterable::ProcessIterator::reference ProcessIterable::ProcessIterator::operator->() 
    {
        return _entry;
    }
    ProcessIterable::ProcessIterator::const_reference ProcessIterable::ProcessIterator::operator*() const 
    {
        return _entry;
    }
    ProcessIterable::ProcessIterator::const_reference ProcessIterable::ProcessIterator::operator->() const
    {
        return _entry;
    }
    ProcessIterable::ProcessIterator ProcessIterable::ProcessIterator::operator++() const
    {
        return ProcessIterator(_snapshot);
    }
    ProcessIterable::ProcessIterator& ProcessIterable::ProcessIterator::end()
    {
        static ProcessIterator empty{};
        return empty;
    }
    bool ProcessIterable::ProcessIterator::IsEqual(ProcessIterator const& other) const
    {
        if (&other == nullptr)
            return false;
        return _snapshot == other._snapshot && 
            (_entry.has_value() == other._entry.has_value() && _entry.value().th32ProcessID == other._entry.value().th32ProcessID) ||
            (!_entry.has_value() && !other._entry.has_value());
    }
    ProcessIterable::ProcessIterator::ProcessIterator(HANDLE snapshot)
        : _snapshot{snapshot}
    {
        PROCESSENTRY32 entry{};
        entry.dwSize = sizeof(PROCESSENTRY32);

        _entry = Process32Next(_snapshot, &entry) == TRUE
            ? optional(entry)
            : nullopt;
    }

    ProcessIterable::ProcessIterator::ProcessIterator()
        : _entry{nullopt}
        , _snapshot{INVALID_HANDLE_VALUE}
    {
    }

    ProcessIterable::iterator ProcessIterable::begin() const noexcept
    {
        return static_cast<bool>(_snapshotHandle)
            ? ProcessIterator(_snapshotHandle.Get())
            : end();
    }

    ProcessIterable::iterator ProcessIterable::end() const noexcept
    {
        return ProcessIterator::end();
    }
    ProcessIterable::const_iterator ProcessIterable::cbegin() const noexcept
    {
        return static_cast<bool>(_snapshotHandle)
            ? ProcessIterator(_snapshotHandle.Get())
            : end();
    }

    ProcessIterable::const_iterator ProcessIterable::cend() const noexcept
    {
        return ProcessIterator::end();
    }

    optional<ProcessIterable> ProcessIterable::GetProcesses()
    {
        try
        {
            return optional( ProcessIterable());
        }
        catch (std::exception&)
        {
            return nullopt;
        }
    }

    ProcessIterable::ProcessIterable()
    {
        PROCESSENTRY32 entry{};
        entry.dwSize = sizeof(PROCESSENTRY32);

        _snapshotHandle.Reset(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
        if (!static_cast<bool>(_snapshotHandle) || Process32First(_snapshotHandle.Get(), &entry) != TRUE)
            throw std::bad_exception();

    }

    bool operator==(ProcessIterable::ProcessIterator const& leftHandSide, ProcessIterable::ProcessIterator const& rightHandSide)
    {
        return &leftHandSide == &rightHandSide ||
            &leftHandSide != nullptr && leftHandSide.IsEqual(rightHandSide);
    }

}
