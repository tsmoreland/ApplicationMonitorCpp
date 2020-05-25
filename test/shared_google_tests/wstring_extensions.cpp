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
#include "shared/string_extensions.h"
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

namespace shared::tests
{
TEST(wide_string, two_parts_with_seperator_in_the_middle)
{
    split_returns_correct_number_of_parts(L"alpha*bravo"s, {L'*'}, 2ULL);
}
TEST(wide_string, three_parts_with_seperator_at_the_start)
{
    split_returns_correct_number_of_parts(L"<alpha<bravo<charlie"s, {L'<'}, 3ULL);
}
TEST(wide_string, correct_three_parts_with_seperator_at_the_start)
{
    split_returns_correct_parts(L"<alpha<bravo<charlie"s, {L'<'}, {L"alpha"s, L"bravo"s, L"charlie"s});
}
TEST(wide_string, four_parts_with_seperator_at_the_end)
{
    split_returns_correct_number_of_parts(L"alpha>bravo>charlie>delta>"s, {L'>'}, 4ULL);
}
TEST(wide_string, correct_four_parts_with_seperator_at_the_end)
{
    split_returns_correct_parts(L"alpha>bravo>charlie>delta>"s, {L'>'}, {L"alpha"s, L"bravo"s, L"charlie"s, L"delta"s});
}
TEST(wide_string, expected_parts_found)
{
    split_returns_correct_parts(L"alpha,bravo,charlie,delta"s, {L','}, {L"alpha"s, L"bravo"s, L"charlie"s, L"delta"s});
}
TEST(wide_string, expected_parts_found_with_multiple_seperators)
{
    split_returns_correct_parts(L"alpha,bravo.charlie.delta,echo"s, {L',', L'.'}, {L"alpha"s, L"bravo"s, L"charlie"s, L"delta"s, L"echo"s});
}
TEST(wide_string, no_parts_when_string_is_empty)
{
    split_returns_correct_parts(L""s, {L',', L'.'}, {});
}
TEST(wide_string, one_part_when_no_seperators)
{
    split_returns_correct_parts(L"alpha,bravo"s, {}, {L"alpha,bravo"s});
}
TEST(wide_string, split_returns_correct_number_of_parts)
{
    split_returns_correct_number_of_parts(L"alpha*bravo"s, {L'*'}, 2ULL);
}
TEST(wide_string, EqualsReturnsTrueForMatchingCaseWhenNotIgnoring)
{
    ASSERT_TRUE(string_equal(L"alpha"s, L"alpha"s, false));
}
TEST(wide_string, EqualsReturnsTrueForMatchingCaseWhenIgnoring)
{
    ASSERT_TRUE(string_equal(L"alpha"s, L"AlphA"s, true));
}
TEST(wide_string, equals_returns_false_forMatchingCaseWhenNotIgnoring)
{
    ASSERT_FALSE(string_equal(L"alpha"s, L"AlphA"s, false));
}
TEST(wide_string, equals_returns_false_when_not_equal_when_not_ignoring_case)
{
    ASSERT_FALSE(string_equal(L"alpha"s, L"Bravo"s, false));
}
TEST(wide_string, equals_returns_false_whennot_equal_when_ignoring_case)
{
    ASSERT_FALSE(string_equal(L"alpha"s, L"Bravo"s, true));
}
TEST(wide_string, equals_returns_true_when_not_ignoring_case_with_wide_string_to_string_comparison)
{
    ASSERT_TRUE(string_equal(L"alpha"s, "alpha"s, false));
}
TEST(wide_string, equals_returns_true_when_ignoring_case_with_wide_string_to_string_comparison)
{
    ASSERT_TRUE(string_equal(L"alpha"s, "AlphA"s, true));
}
TEST(wide_string, equals_returns_false_for_matching_case_when_not_ignoring_with_wide_string_to_string_comparison)
{
    ASSERT_FALSE(string_equal(L"alpha"s, "AlphA"s, false));
}
TEST(wide_string, equals_returns_false_whennot_equal_when_not_ignorning_case_with_wide_string_to_string_comparison)
{
    ASSERT_FALSE(string_equal(L"alpha"s, "Bravo"s, false));
}
TEST(wide_string, equals_returns_false_whennot_equal_when_ignoring_case_with_wide_string_to_string_comparison)
{
    ASSERT_FALSE(string_equal(L"alpha"s, "Bravo"s, true));
}

TEST(wide_string, returns_true_when_contains_single_part)
{
    ASSERT_TRUE(string_contains_in_order(L"abcdef"s,  vector<wstring>{L"bc"s}));
}
TEST(wide_string, returns_false_when_contains_single_part)
{
    ASSERT_FALSE(string_contains_in_order(L"abcdef"s,  vector<wstring>{L"xy"s}));
}
TEST(wide_string, returns_true_when_no_parts_given)
{
    vector<wstring> const parts{};
    ASSERT_TRUE(string_contains_in_order(L"abcdef"s, parts));
}

TEST(wide_string, returns_true_when_contains_multiple_parts)
{
    ASSERT_TRUE(string_contains_in_order(L"abcdef"s, vector<wstring>{L"bc"s, L"de"s}));
}
TEST(wide_string, returns_false_when_does_not_contains_multiple_parts_when_out_of_order)
{
    ASSERT_FALSE(string_contains_in_order(L"abcdef"s,  vector<wstring>{L"de"s, L"bc"s}));
}

}
