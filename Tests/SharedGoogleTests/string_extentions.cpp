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
    TEST(String, TwoPartsWithSeperatorInTheMiddle) {
        SplitReturnsCorrectNumberOfParts("alpha*bravo"s, {'*'}, 2ULL);
    }
    TEST(String, ThreePartsWithSeperatorAtTheStart) {
        SplitReturnsCorrectNumberOfParts("<alpha<bravo<charlie"s, {'<'}, 3ULL);
    }
    TEST(String, CorrectThreePartsWithSeperatorAtTheStart) {
        SplitReturnsCorrectCorrectParts("<alpha<bravo<charlie"s, {'<'}, {"alpha"s, "bravo"s, "charlie"s});
    }
    TEST(String, FourPartsWithSeperatorAtTheEnd) {
        SplitReturnsCorrectNumberOfParts("alpha>bravo>charlie>delta>"s, {'>'}, 4ULL);
    }
    TEST(String, CorrectFourPartsWithSeperatorAtTheEnd) {
        SplitReturnsCorrectCorrectParts("alpha>bravo>charlie>delta>"s, {'>'}, {"alpha"s, "bravo"s, "charlie"s, "delta"s});
    }
    TEST(String, ExpectedPartsFound) {
        SplitReturnsCorrectCorrectParts("alpha,bravo,charlie,delta"s, {','}, {"alpha"s, "bravo"s, "charlie"s, "delta"s});
    }
    TEST(String, ExpectedPartsFoundWithMultipleSeperators) {
        SplitReturnsCorrectCorrectParts("alpha,bravo.charlie.delta,echo"s, {',', '.'}, {"alpha"s, "bravo"s, "charlie"s, "delta"s, "echo"s});
    }
    TEST(String, NoPartsWhenStringIsEmpty) {
        SplitReturnsCorrectCorrectParts(""s, {',', '.'}, {});
    }
    TEST(String, OnePartWhenNoSeperators) {
        SplitReturnsCorrectCorrectParts("alpha,bravo"s, {}, {"alpha,bravo"s});
    }
    TEST(String, SplitReturnsCorrectNumberOfParts) {
        SplitReturnsCorrectNumberOfParts("alpha*bravo"s, {'*'}, 2ULL);
    }
    TEST(String, EqualsReturnsTrueForMatchingCaseWhenNotIgnoring) {
        ASSERT_TRUE(string_equal("alpha"s, "alpha"s, false));
    }
    TEST(String, EqualsReturnsTrueForMatchingCaseWhenIgnoring) {
        ASSERT_TRUE(string_equal("alpha"s, "AlphA"s, true));
    }
    TEST(String, EqualsReturnsFalseForMatchingCaseWhenNotIgnoring) {
        ASSERT_FALSE(string_equal("alpha"s, "AlphA"s, false));
    }
    TEST(String, EqualsReturnsFalseWhenNotEqualWhenNotIgnoringCase) {
        ASSERT_FALSE(string_equal("alpha"s, "Bravo"s, false));
    }
    TEST(String, EqualsReturnsFalseWhenNotEqualWhenIgnoringCase) {
        ASSERT_FALSE(string_equal("alpha"s, "Bravo"s, true));
    }
    TEST(String, EqualsReturnsTrueWhenNotIgnoringCaseWithStringToWideComparison) {
        ASSERT_TRUE(string_equal("alpha"s, L"alpha"s, false));
    }
    TEST(String, EqualsReturnsTrueWhenIgnoringCaseWithStringToWideComparison) {
        ASSERT_TRUE(string_equal("alpha"s, L"AlphA"s, true));
    }
    TEST(String, EqualsReturnsFalseForMatchingCaseWhenNotIgnoringWithStringToWideComparison) {
        ASSERT_FALSE(string_equal("alpha"s, L"AlphA"s, false));
    }
    TEST(String, EqualsReturnsFalseWhenNotEqualWhenNotIgnoringCaseWithStringToWideComparison) {
        ASSERT_FALSE(string_equal("alpha"s, L"Bravo"s, false));
    }
    TEST(String, EqualsReturnsFalseWhenNotEqualWhenIgnoringCaseWithStringToWideComparison) {
        ASSERT_FALSE(string_equal("alpha"s, L"Bravo"s, true));
    }

    TEST(String, ReturnsTrueWhenContainsSinglePart) {
        ASSERT_TRUE(string_contains_in_order("abcdef"s,  vector<string>{"bc"s}));
    }
    TEST(String, ReturnsFalseWhenContainsSinglePart) {
        ASSERT_FALSE(string_contains_in_order("abcdef"s,  vector<string>{"xy"s}));
    }
    TEST(String, ReturnsTrueWhenNoPartsGiven) {
        vector<string> const parts{};
        ASSERT_TRUE(string_contains_in_order("abcdef"s, parts));
    }

    TEST(String, ReturnsTrueWhenContainsMultipleParts) {
        ASSERT_TRUE(string_contains_in_order("abcdef"s,  vector<string>{"bc"s, "de"s}));
    }
    TEST(String, ReturnsFalseWhenDoesNotContainsMultiplePartsWhenOutOfOrder) {
        ASSERT_FALSE(string_contains_in_order("abcdef"s,  vector<string>{"de"s, "bc"s}));
    }

}