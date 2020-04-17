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
#include <tlhelp32.h>

namespace Shared::Infrastructure
{
    class Process;

    class ProcessIterable final
    {
    public:
        class ProcessIterator final
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::optional<PROCESSENTRY32>;
            using difference_type = std::allocator_traits<std::optional<PROCESSENTRY32>>::difference_type;
            using pointer = std::allocator_traits<std::optional<PROCESSENTRY32>>::pointer;
            using reference = std::optional<PROCESSENTRY32>&;
            using const_reference = std::optional<PROCESSENTRY32> const&;

            [[nodiscard]] reference operator*();
            [[nodiscard]] reference operator->();
            [[nodiscard]] const_reference operator*() const;
            [[nodiscard]] const_reference operator->() const;
            [[nodiscard]] ProcessIterator operator++() const;

            static ProcessIterator& end();
            [[nodiscard]] bool IsEqual(ProcessIterator const& other) const;

            friend class ProcessIterable;
        private:
            explicit ProcessIterator(HANDLE snapshot);
            explicit ProcessIterator();

            std::optional<PROCESSENTRY32> _entry{};
            HANDLE _snapshot;
        };

        using value_type = std::optional<PROCESSENTRY32>;
        using iterator = ProcessIterator;
        using const_iterator = ProcessIterator const;

        [[nodiscard]] iterator begin() const noexcept;
        [[nodiscard]] iterator end() const noexcept;
        [[nodiscard]] const_iterator cbegin() const noexcept;
        [[nodiscard]] const_iterator cend() const noexcept;

        [[nodiscard]] static std::optional<ProcessIterable> GetProcesses();

    private:
        explicit ProcessIterable();

        Shared::Infrastructure::HandleWithInvalidForEmpty _snapshotHandle;

    };

    [[nodiscard]] bool operator==(ProcessIterable::ProcessIterator const& leftHandSide, ProcessIterable::ProcessIterator const& rightHandSide);


    /* eventual goal:

#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>

int main( int, char *[] )
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    // begin starts here, though maybe use NullHandle
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (stricmp(entry.szExeFile, "target.exe") == 0)
            {  
                // this here is the result of begin()
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

                // Do stuff..

                CloseHandle(hProcess);
            }

            // end might be more complex, either we use shared_ptr{} or create an Empty Process iterator (null object pattern)
        }
    }

    CloseHandle(snapshot);

    return 0;
}

    */
}
