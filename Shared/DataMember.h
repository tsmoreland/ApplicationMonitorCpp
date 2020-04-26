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

namespace Shared::Infrastructure {
    template<typename OWNER_TYPE, typename VALUE_TYPE>
    struct DataMember {

        constexpr DataMember(VALUE_TYPE OWNER_TYPE::*member, char const*const name)
            : Member(member), Name(name) {
        }
        using Type = VALUE_TYPE;
        VALUE_TYPE OWNER_TYPE::*Member;
        char const*const Name;
    };

    template<typename OWNER_TYPE, typename VALUE_TYPE>
    constexpr auto property(VALUE_TYPE OWNER_TYPE::*member, char const*const name) {
        return DataMemberImpl(member, name);
    }

}

