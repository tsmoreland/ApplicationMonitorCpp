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
#include "string_extensions_common.h"

using std::basic_string;
using std::basic_string_view;
using std::equal;
using std::string;
using std::string_view;
using std::vector;
using std::wstring;
using std::wstring_view;

using extension::string_equal;
using extension::string_split;
using extension::string_contains_in_order;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
#pragma warning(pop)

namespace Shared::Tests
{
    TEST(WideString, TwoPartsWithSeperatorInTheMiddle)
    {
        SplitReturnsCorrectNumberOfParts(L"alpha*bravo"s, {L'*'}, 2ULL);
    }
    TEST(WideString, ThreePartsWithSeperatorAtTheStart)
    {
        SplitReturnsCorrectNumberOfParts(L"<alpha<bravo<charlie"s, {L'<'}, 3ULL);
    }
    TEST(WideString, CorrectThreePartsWithSeperatorAtTheStart)
    {
        SplitReturnsCorrectCorrectParts(L"<alpha<bravo<charlie"s, {L'<'}, {L"alpha"s, L"bravo"s, L"charlie"s});
    }
    TEST(WideString, FourPartsWithSeperatorAtTheEnd)
    {
        SplitReturnsCorrectNumberOfParts(L"alpha>bravo>charlie>delta>"s, {L'>'}, 4ULL);
    }
    TEST(WideString, CorrectFourPartsWithSeperatorAtTheEnd)
    {
        SplitReturnsCorrectCorrectParts(L"alpha>bravo>charlie>delta>"s, {L'>'}, {L"alpha"s, L"bravo"s, L"charlie"s, L"delta"s});
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
    TEST(WideString, EqualsReturnsTrueWhenNotIgnoringCaseWithWideStringToStringComparison)
    {
        ASSERT_TRUE(string_equal(L"alpha"s, "alpha"s, false));
    }
    TEST(WideString, EqualsReturnsTrueWhenIgnoringCaseWithWideStringToStringComparison)
    {
        ASSERT_TRUE(string_equal(L"alpha"s, "AlphA"s, true));
    }
    TEST(WideString, EqualsReturnsFalseForMatchingCaseWhenNotIgnoringWithWideStringToStringComparison)
    {
        ASSERT_FALSE(string_equal(L"alpha"s, "AlphA"s, false));
    }
    TEST(WideString, EqualsReturnsFalseWhenNotEqualWhenNotIgnoringCaseWithWideStringToStringComparison)
    {
        ASSERT_FALSE(string_equal(L"alpha"s, "Bravo"s, false));
    }
    TEST(WideString, EqualsReturnsFalseWhenNotEqualWhenIgnoringCaseWithWideStringToStringComparison)
    {
        ASSERT_FALSE(string_equal(L"alpha"s, "Bravo"s, true));
    }

    TEST(WideString, ReturnsTrueWhenContainsSinglePart)
    {
        ASSERT_TRUE(string_contains_in_order(L"abcdef"s,  vector<wstring>{L"bc"s}));
    }
    TEST(WideString, ReturnsFalseWhenContainsSinglePart)
    {
        ASSERT_FALSE(string_contains_in_order(L"abcdef"s,  vector<wstring>{L"xy"s}));
    }
    TEST(WideString, ReturnsTrueWhenNoPartsGiven)
    {
        vector<wstring> const parts{};
        ASSERT_TRUE(string_contains_in_order(L"abcdef"s, parts));
    }

    TEST(WideString, ReturnsTrueWhenContainsMultipleParts)
    {
        ASSERT_TRUE(string_contains_in_order(L"abcdef"s,  vector<wstring>{L"bc"s, L"de"s}));
    }
    TEST(WideString, ReturnsFalseWhenDoesNotContainsMultiplePartsWhenOutOfOrder)
    {
        ASSERT_FALSE(string_contains_in_order(L"abcdef"s,  vector<wstring>{L"de"s, L"bc"s}));
    }

}
