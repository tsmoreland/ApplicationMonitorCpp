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

#include <filesystem>
#include <vector>

namespace shared::tests
{

template <class PREDICATE>
std::vector<std::filesystem::path> populate_expected_files(std::filesystem::path const& folder, PREDICATE predicate)
{
    std::vector<std::filesystem::path> expected;
    if (std::filesystem::exists(folder) && std::filesystem::is_directory(folder)) {
        auto const expectedFiles = std::filesystem::directory_iterator(folder);
        std::copy_if(begin(expectedFiles), end(expectedFiles), back_inserter(expected), 
            [predicate](std::filesystem::directory_entry const& entry) {
                return entry.is_regular_file() && predicate(entry);
            });
    }
    return expected;
}

}
