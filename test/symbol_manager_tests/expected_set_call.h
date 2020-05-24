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

namespace symbol_manager::test {

    struct expected_set_call
    {
        Cardinality cardinality;
        std::string value;
        bool success;

        void swap(expected_set_call& other) noexcept {
            ::swap(cardinality, other.cardinality);
            ::swap(value, other.value);
            ::swap(success, other.success);
        }
    };

    inline void swap(expected_set_call& left, expected_set_call& right) noexcept
    {
        left.swap(right);
    }
    inline expected_set_call successfully_set_to(string value, optional<Cardinality> const& cardinality = nullopt) {
        return { cardinality.value_or(AnyNumber()), move(value), true };
    }

}
