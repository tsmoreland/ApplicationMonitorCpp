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
#include "string_extensions.h"

using std::basic_string;
using std::basic_string_view;
using std::equal;
using std::string;
using std::vector;
using std::wstring;

using extension::string_equal;
using extension::string_split;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
#pragma warning(pop)

namespace Shared::Tests
{
    template<typename TCHAR>
    void SplitReturnsCorrectNumberOfParts(basic_string<TCHAR> value, vector<TCHAR> seperators, size_t const expected);
    template <typename TCHAR>
    void SplitReturnsCorrectCorrectParts(basic_string<TCHAR> value, vector<TCHAR> seperators, vector<basic_string<TCHAR>> const& expected_parts);

    TEST(String, TwoPartsWithSeperatorInTheMiddle)
    {
        SplitReturnsCorrectNumberOfParts("alpha*bravo"s, {'*'}, 2ULL);
    }
    TEST(String, TwoPartsWithSeperatorAtTheStart)
    {
        SplitReturnsCorrectNumberOfParts("<alpha<bravo<charlie"s, {'<'}, 3ULL);
    }
    TEST(String, TwoPartsWithSeperatorAtTheEnd)
    {
        SplitReturnsCorrectNumberOfParts("alpha>bravo>charlie>delta>"s, {'>'}, 4ULL);
    }
    TEST(String, ExpectedPartsFound)
    {
        SplitReturnsCorrectCorrectParts("alpha,bravo,charlie,delta"s, {','}, {"alpha"s, "bravo"s, "charlie"s, "delta"s});
    }
    TEST(String, ExpectedPartsFoundWithMultipleSeperators)
    {
        SplitReturnsCorrectCorrectParts("alpha,bravo.charlie.delta,echo"s, {',', '.'}, {"alpha"s, "bravo"s, "charlie"s, "delta"s, "echo"s});
    }
    TEST(String, NoPartsWhenStringIsEmpty)
    {
        SplitReturnsCorrectCorrectParts(""s, {',', '.'}, {});
    }
    TEST(String, OnePartWhenNoSeperators)
    {
        SplitReturnsCorrectCorrectParts("alpha,bravo"s, {}, {"alpha,bravo"s});
    }
    TEST(String, SplitReturnsCorrectNumberOfParts)
    {
        SplitReturnsCorrectNumberOfParts("alpha*bravo"s, {'*'}, 2ULL);
    }
    TEST(String, EqualsReturnsTrueForMatchingCaseWhenNotIgnoring)
    {
        ASSERT_TRUE(string_equal("alpha"s, "alpha"s, false));
    }
    TEST(String, EqualsReturnsTrueForMatchingCaseWhenIgnoring)
    {
        ASSERT_TRUE(string_equal("alpha"s, "AlphA"s, true));
    }
    TEST(String, EqualsReturnsFalseForMatchingCaseWhenNotIgnoring)
    {
        ASSERT_FALSE(string_equal("alpha"s, "AlphA"s, false));
    }
    TEST(String, EqualsReturnsFalseWhenNotEqualWhenNotIgnoringCase)
    {
        ASSERT_FALSE(string_equal("alpha"s, "Bravo"s, false));
    }
    TEST(String, EqualsReturnsFalseWhenNotEqualWhenIgnoringCase)
    {
        ASSERT_FALSE(string_equal("alpha"s, "Bravo"s, true));
    }
    TEST(String, EqualsReturnsTrueWhenNotIgnoringCaseWithStringToWideComparision)
    {
        ASSERT_TRUE(string_equal("alpha"s, L"alpha"s, false));
    }
    TEST(String, EqualsReturnsTrueWhenIgnoringCaseWithStringToWideComparision)
    {
        ASSERT_TRUE(string_equal("alpha"s, L"AlphA"s, true));
    }
    TEST(String, EqualsReturnsFalseForMatchingCaseWhenNotIgnoringWithStringToWideComparision)
    {
        ASSERT_FALSE(string_equal("alpha"s, L"AlphA"s, false));
    }
    TEST(String, EqualsReturnsFalseWhenNotEqualWhenNotIgnoringCaseWithStringToWideComparision)
    {
        ASSERT_FALSE(string_equal("alpha"s, L"Bravo"s, false));
    }
    TEST(String, EqualsReturnsFalseWhenNotEqualWhenIgnoringCaseWithStringToWideComparision)
    {
        ASSERT_FALSE(string_equal("alpha"s, L"Bravo"s, true));
    }

    TEST(WideString, TwoPartsWithSeperatorInTheMiddle)
    {
        SplitReturnsCorrectNumberOfParts(L"alpha*bravo"s, {L'*'}, 2ULL);
    }
    TEST(WideString, TwoPartsWithSeperatorAtTheStart)
    {
        SplitReturnsCorrectNumberOfParts(L"<alpha<bravo<charlie"s, {L'<'}, 3ULL);
    }
    TEST(WideString, TwoPartsWithSeperatorAtTheEnd)
    {
        SplitReturnsCorrectNumberOfParts(L"alpha>bravo>charlie>delta>"s, {L'>'}, 4ULL);
    }
    TEST(WideString, ExpectedPartsFound)
    {
        SplitReturnsCorrectCorrectParts(L"alpha,bravo,charlie,delta"s, {L','}, {L"alpha"s, L"bravo"s, L"charlie"s, L"delta"s});
    }
    TEST(WideString, ExpectedPartsFoundWithMultipleSeperators)
    {
        SplitReturnsCorrectCorrectParts(L"alpha,bravo.charlie.delta,echo"s, {L',', L'.'}, {L"alpha"s, L"bravo"s, L"charlie"s, L"delta"s, L"echo"s});
    }
    TEST(WideString, NoPartsWhenStringIsEmpty)
    {
        SplitReturnsCorrectCorrectParts(L""s, {L',', L'.'}, {});
    }
    TEST(WideString, OnePartWhenNoSeperators)
    {
        SplitReturnsCorrectCorrectParts(L"alpha,bravo"s, {}, {L"alpha,bravo"s});
    }
    TEST(WideString, SplitReturnsCorrectNumberOfParts)
    {
        SplitReturnsCorrectNumberOfParts(L"alpha*bravo"s, {L'*'}, 2ULL);
    }
    TEST(WideString, EqualsReturnsTrueForMatchingCaseWhenNotIgnoring)
    {
        ASSERT_TRUE(string_equal(L"alpha"s, L"alpha"s, false));
    }
    TEST(WideString, EqualsReturnsTrueForMatchingCaseWhenIgnoring)
    {
        ASSERT_TRUE(string_equal(L"alpha"s, L"AlphA"s, true));
    }
    TEST(WideString, EqualsReturnsFalseForMatchingCaseWhenNotIgnoring)
    {
        ASSERT_FALSE(string_equal(L"alpha"s, L"AlphA"s, false));
    }
    TEST(WideString, EqualsReturnsFalseWhenNotEqualWhenNotIgnoringCase)
    {
        ASSERT_FALSE(string_equal(L"alpha"s, L"Bravo"s, false));
    }
    TEST(WideString, EqualsReturnsFalseWhenNotEqualWhenIgnoringCase)
    {
        ASSERT_FALSE(string_equal(L"alpha"s, L"Bravo"s, true));
    }
    TEST(WideString, EqualsReturnsTrueWhenNotIgnoringCaseWithWideStringToStringComparision)
    {
        ASSERT_TRUE(string_equal(L"alpha"s, "alpha"s, false));
    }
    TEST(WideString, EqualsReturnsTrueWhenIgnoringCaseWithWideStringToStringComparision)
    {
        ASSERT_TRUE(string_equal(L"alpha"s, "AlphA"s, true));
    }
    TEST(WideString, EqualsReturnsFalseForMatchingCaseWhenNotIgnoringWithWideStringToStringComparision)
    {
        ASSERT_FALSE(string_equal(L"alpha"s, "AlphA"s, false));
    }
    TEST(WideString, EqualsReturnsFalseWhenNotEqualWhenNotIgnoringCaseWithWideStringToStringComparision)
    {
        ASSERT_FALSE(string_equal(L"alpha"s, "Bravo"s, false));
    }
    TEST(WideString, EqualsReturnsFalseWhenNotEqualWhenIgnoringCaseWithWideStringToStringComparision)
    {
        ASSERT_FALSE(string_equal(L"alpha"s, "Bravo"s, true));
    }

    template <typename TCHAR>
    void SplitReturnsCorrectNumberOfParts(basic_string<TCHAR> value, vector<TCHAR> seperators, size_t const expected)
    {
        // Act
        auto const parts = string_split<TCHAR>(value, seperators);;

        // Assert
        ASSERT_EQ(expected, parts.size());
    }
    template <typename TCHAR>
    void SplitReturnsCorrectCorrectParts(basic_string<TCHAR> value, vector<TCHAR> seperators, vector<basic_string<TCHAR>> const& expected_parts)
    {
        // Act
        auto const parts = string_split<TCHAR>(value, seperators);;

        // Assert
        ASSERT_TRUE(equal(begin(expected_parts), end(expected_parts), begin(parts), [](auto const& lhs, auto const& rhs) 
        { 
            return equal(begin(lhs), end(lhs), begin(rhs));
        }));
    }


}