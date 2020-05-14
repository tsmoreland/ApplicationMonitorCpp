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

namespace shared::tests
{
TEST(string, two_parts_with_seperator_in_the_middle)
{
    split_returns_correct_number_of_parts("alpha*bravo"s, {'*'}, 2ULL);
}
TEST(string, three_parts_with_seperator_at_the_start)
{
    split_returns_correct_number_of_parts("<alpha<bravo<charlie"s, {'<'}, 3ULL);
}
TEST(string, correct_three_parts_with_seperator_at_the_start)
{
    split_returns_correct_parts("<alpha<bravo<charlie"s, {'<'}, {"alpha"s, "bravo"s, "charlie"s});
}
TEST(string, four_parts_with_seperator_at_the_end)
{
    split_returns_correct_number_of_parts("alpha>bravo>charlie>delta>"s, {'>'}, 4ULL);
}
TEST(string, correct_four_parts_with_seperator_at_the_end)
{
    split_returns_correct_parts("alpha>bravo>charlie>delta>"s, {'>'}, {"alpha"s, "bravo"s, "charlie"s, "delta"s});
}
TEST(string, expected_parts_found)
{
    split_returns_correct_parts("alpha,bravo,charlie,delta"s, {','}, {"alpha"s, "bravo"s, "charlie"s, "delta"s});
}
TEST(string, expected_parts_found_with_multiple_seperators)
{
    split_returns_correct_parts("alpha,bravo.charlie.delta,echo"s, {',', '.'}, {"alpha"s, "bravo"s, "charlie"s, "delta"s, "echo"s});
}
TEST(string, no_parts_when_string_is_empty)
{
    split_returns_correct_parts(""s, {',', '.'}, {});
}
TEST(string, one_part_when_no_seperators)
{
    split_returns_correct_parts("alpha,bravo"s, {}, {"alpha,bravo"s});
}
TEST(string, split_returns_correct_number_of_parts)
{
    split_returns_correct_number_of_parts("alpha*bravo"s, {'*'}, 2ULL);
}
TEST(string, equals_returns_true_for_matching_case_when_not_ignoring)
{
    ASSERT_TRUE(string_equal("alpha"s, "alpha"s, false));
}
TEST(string, equals_returns_true_for_matching_case_when_ignoring)
{
    ASSERT_TRUE(string_equal("alpha"s, "AlphA"s, true));
}
TEST(string, equals_returns_false_for_matching_case_when_not_ignoring)
{
    ASSERT_FALSE(string_equal("alpha"s, "AlphA"s, false));
}
TEST(string, equals_returns_false_when_not_equal_when_not_ignoring_case)
{
    ASSERT_FALSE(string_equal("alpha"s, "Bravo"s, false));
}
TEST(string, equals_returns_false_when_not_equal_when_ignoring_case)
{
    ASSERT_FALSE(string_equal("alpha"s, "Bravo"s, true));
}
TEST(string, equals_returns_true_when_not_ignoring_case_with_string_to_wide_comparison)
{
    ASSERT_TRUE(string_equal("alpha"s, L"alpha"s, false));
}
TEST(string, equals_returns_true_when_ignoring_case_with_string_to_wide_comparison)
{
    ASSERT_TRUE(string_equal("alpha"s, L"AlphA"s, true));
}
TEST(string, equals_returns_false_for_matching_case_when_not_ignoring_with_string_to_wide_comparison)
{
    ASSERT_FALSE(string_equal("alpha"s, L"AlphA"s, false));
}
TEST(string, equals_returns_false_when_not_equal_when_not_ignoring_case_with_string_to_wide_comparison)
{
    ASSERT_FALSE(string_equal("alpha"s, L"Bravo"s, false));
}
TEST(string, equals_returns_false_when_not_equal_when_ignoring_case_with_string_to_wide_comparison)
{
    ASSERT_FALSE(string_equal("alpha"s, L"Bravo"s, true));
}

TEST(string, returns_true_when_contains_single_part)
{
    ASSERT_TRUE(string_contains_in_order("abcdef"s,  vector<string>{"bc"s}));
}
TEST(string, returns_false_when_contains_single_part)
{
    ASSERT_FALSE(string_contains_in_order("abcdef"s,  vector<string>{"xy"s}));
}
TEST(string, returns_true_when_no_parts_given)
{
    vector<string> const parts{};
    ASSERT_TRUE(string_contains_in_order("abcdef"s, parts));
}

TEST(string, returns_true_when_contains_multiple_parts)
{
    ASSERT_TRUE(string_contains_in_order("abcdef"s, vector<string>{"bc"s, "de"s}));
}
TEST(string, returns_false_when_does_not_contains_multiple_parts_when_out_of_order)
{
    ASSERT_FALSE(string_contains_in_order("abcdef"s,  vector<string>{"de"s, "bc"s}));
}

}