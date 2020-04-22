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

#include <string_view>
#include <algorithm>
#include <locale>

namespace std
{
    template <typename TCHAR>
    [[nodiscard]] bool string_equal(std::basic_string_view<TCHAR> const& leftHandSide, std::basic_string_view<TCHAR> const &rightHandSide, bool ignoreCase = false)
    {
        if (&leftHandSide == &rightHandSide)
            return true;
        if (&leftHandSide == nullptr)
            return false;

        if (!ignoreCase)
            return leftHandSide == rightHandSide;

        const auto locale = std::locale();

        auto pred = [l = static_cast<const std::locale&>(locale)](TCHAR const& lhs, TCHAR const& rhs) -> bool
        {
            return std::toupper(lhs, l) == std::toupper(rhs, l);
        };

        return std::equal(
            begin(leftHandSide), end(leftHandSide), 
            begin(rightHandSide), end(rightHandSide), 
            pred);
    }
    [[nodiscard]] inline bool string_equal(std::string_view const leftHandSide, std::wstring_view const rightHandSide, bool ignoreCase /*= false */)
    {
        if constexpr (&leftHandSide == nullptr && &rightHandSide == nullptr)
            return true;
        if constexpr (&leftHandSide == nullptr)
            return false;

        const auto locale = std::locale();

        if (ignoreCase)
        {
            auto pred = [l = static_cast<const std::locale&>(locale)](char const& lhs, wchar_t const& rhs) -> bool
            {
                const wchar_t wideLhs = std::toupper(lhs, l);
                return wideLhs == std::toupper(rhs, l);
            };

            return std::equal(
                begin(leftHandSide), end(leftHandSide), 
                begin(rightHandSide), end(rightHandSide), 
                pred);
        }
        else
        {
            auto pred = [](char const& lhs, wchar_t const& rhs) -> bool
            {
                const wchar_t wideLhs = lhs;
                return wideLhs == rhs;
            };

            return std::equal(
                begin(leftHandSide), end(leftHandSide), 
                begin(rightHandSide), end(rightHandSide), 
                pred);
        }
    }
    [[nodiscard]] inline bool string_equal(std::wstring_view const leftHandSide, std::string_view const rightHandSide, bool ignoreCase /*= false */)
    {
        return string_equal(rightHandSide, leftHandSide, ignoreCase);
    }

    template <typename TCHAR>
    [[nodiscard]] std::vector<std::basic_string_view<TCHAR>> string_split(std::basic_string_view<TCHAR> const& value, std::vector<TCHAR> const& seperators)
    {
        if (seperators.empty() || value.empty())
            return std::vector<std::basic_string_view<TCHAR>>();

        std::vector<std::basic_string_view<TCHAR>> parts{};

        size_t currentIndex{};

        for (size_t i = 0, length = value.size(), seperatorCount = seperators.size(); i < length; i++)
        {
            auto const& match = std::find_if(begin(seperators), end(seperators), [valueAtIndex = value[i]](auto const& seperator)
            {
                return seperator == valueAtIndex;
            });
            if (match == end(seperators))
                continue;
            if (auto const partLength = currentIndex + (i - currentIndex); partLength > 1)
            {
                parts.push_back(value.substr(currentIndex, partLength));
                currentIndex = i + 1;
                break;
            }
        }

        return parts;
    }

}

