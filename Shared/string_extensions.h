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
    [[nodiscard]] bool string_equal(std::basic_string_view<TCHAR> const left_hand_side, std::basic_string_view<TCHAR> const right_hand_side, bool const ignoreCase = false)
    {
        if (&left_hand_side == &right_hand_side)
            return true;
        if (&left_hand_side == nullptr)
            return false;

        if (!ignoreCase)
            return left_hand_side == right_hand_side;

        const auto locale = std::locale();

        auto pred = [l = static_cast<const std::locale&>(locale)](TCHAR const& lhs, TCHAR const& rhs) -> bool
        {
            return std::toupper(lhs, l) == std::toupper(rhs, l);
        };

        return std::equal(
            begin(left_hand_side), end(left_hand_side), 
            begin(right_hand_side), end(right_hand_side), 
            pred);
    }
    template <typename TCHAR>
    [[nodiscard]] bool string_equal(std::basic_string<TCHAR> const& left_hand_side, std::basic_string<TCHAR> const &right_hand_side, bool const ignoreCase = false)
    {
        std::basic_string_view<TCHAR> const leftView(left_hand_side);
        std::basic_string_view<TCHAR> const rightView(right_hand_side);
        return string_equal(leftView, rightView, ignoreCase);
    }

    [[nodiscard]] inline bool string_equal(std::string_view const left_hand_side, std::wstring_view const right_hand_side, bool const ignoreCase = false)
    {
        const auto locale = std::locale();

        if (ignoreCase)
        {
            auto pred = [l = static_cast<const std::locale&>(locale)](char const& lhs, wchar_t const& rhs) -> bool
            {
                const wchar_t wideLhs = std::toupper(lhs, l);
                return wideLhs == std::toupper(rhs, l);
            };

            return std::equal(
                begin(left_hand_side), end(left_hand_side), 
                begin(right_hand_side), end(right_hand_side), 
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
                begin(left_hand_side), end(left_hand_side), 
                begin(right_hand_side), end(right_hand_side), 
                pred);
        }
    }
    [[nodiscard]] inline bool string_equal(std::wstring_view const left_hand_side, std::string_view const right_hand_side, bool const ignoreCase = false)
    {
        return string_equal(right_hand_side, left_hand_side, ignoreCase);
    }

    [[nodiscard]] inline bool string_equal(std::string const& left_hand_side, std::wstring const& right_hand_side, bool const ignoreCase = false)
    {
        std::string_view const leftView(left_hand_side);
        std::wstring_view const rightView(right_hand_side);
        return string_equal(leftView, rightView, ignoreCase);
    }
    [[nodiscard]] inline bool string_equal(std::wstring const& left_hand_side, std::string const& right_hand_side, bool const ignoreCase = false)
    {
        return string_equal(right_hand_side, left_hand_side, ignoreCase);
    }

    template <typename TCHAR>
    [[nodiscard]] std::vector<std::basic_string_view<TCHAR>> string_split(std::basic_string_view<TCHAR> const& value, std::vector<TCHAR> const& seperators)
    {
        if (value.empty())
            return std::vector<std::basic_string_view<TCHAR>>();
        if (seperators.empty())
            return std::vector<std::basic_string_view<TCHAR>>{value};

        std::vector<std::basic_string_view<TCHAR>> parts{};

        size_t currentIndex{};
        size_t i{0};
        size_t const length{value.size()};

        for (; i < length; i++)
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
            }
        }
        if (currentIndex < length - 1)
            parts.push_back(value.substr(currentIndex,  length - currentIndex));
        return parts;
    }

}

