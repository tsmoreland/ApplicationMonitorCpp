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

namespace Shared::Infrastructure
{
    /*
    class Process;

    class ProcessIterable final
    {
    public:
        class ProcessIterator final
        {
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::shared_ptr<Process>;
            using difference_type = std::allocator_traits<std::shared_ptr<Process>>::difference_type;
            using pointer = std::allocator_traits<std::shared_ptr<Process>>::pointer;
            using reference = std::shared_ptr<Process>&;

            [[nodiscard]] reference operator*() const;
            [[nodiscard]] reference operator->() const;
            [[nodiscard]] ProcessIterator operator++() const;
        };

        using value_type = std::shared_ptr<Process>;
        using iterator = ProcessIterator;
        using const_iterator = const ProcessIterator;

        [[nodiscard]] iterator begin() const noexcept;
        [[nodiscard]] iterator end() const noexcept;
    };
    */

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
