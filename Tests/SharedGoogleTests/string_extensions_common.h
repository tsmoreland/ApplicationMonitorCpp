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


namespace Shared::Tests
{
    template<typename TCHAR>
    void SplitReturnsCorrectNumberOfParts(std::basic_string<TCHAR> value, std::vector<TCHAR> seperators, size_t const expected)
    {
        // Act
        auto const parts = extension::string_split<TCHAR>(value, seperators);;

        // Assert
        ASSERT_EQ(expected, parts.size());
    }

    template <typename TCHAR>
    void SplitReturnsCorrectCorrectParts(std::basic_string<TCHAR> value, std::vector<TCHAR> seperators, std::vector<std::basic_string<TCHAR>> const& expected_parts)
    {
        // Act
        auto const parts = extension::string_split<TCHAR>(value, seperators);;

        // Assert
        ASSERT_TRUE(equal(begin(expected_parts), end(expected_parts), begin(parts), 
            [](auto const& lhs, auto const& rhs) 
            { 
                return equal(begin(lhs), end(lhs), begin(rhs));
            }));
    }

}

