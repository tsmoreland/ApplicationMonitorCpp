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

#include <new>

namespace shared::infrastructure
{

    template <typename TRAITS>
    class unique_handle final
    {
        using Pointer = typename TRAITS::Pointer;
    public:
        [[nodiscard]] Pointer Get() const noexcept {
            return value;
        }
        [[maybe_unused]] Pointer Release() noexcept {
            auto oldValue = value;
            value = TRAITS::Invalid();
            return oldValue;
        }
        [[maybe_unused]] bool Reset(Pointer newValue = TRAITS::Invalid()) noexcept {
            if (value == newValue)
                return static_cast<bool>(*this);

            Close();
            value = newValue;
            return static_cast<bool>(*this);
        }
        explicit operator bool() const noexcept {
            return value != TRAITS::Invalid();
        }

        explicit unique_handle(Pointer value = TRAITS::Invalid()) noexcept
            : value{value} {
        }
        unique_handle(const unique_handle&) = delete;
        unique_handle& operator=(const unique_handle&) = delete;
        unique_handle(unique_handle&& other) noexcept
            : value{std::move(other.Release())} {
        }
        unique_handle& operator=(unique_handle&& other) noexcept {
            if (this != &other)
                Reset(other.Release());
            return *this;
        }
        ~unique_handle() {
            Close();
        }
        void swap(unique_handle<TRAITS>& other) noexcept {
            std::swap(value, other.value);
        }

    private:
        Pointer value;

        void Close() noexcept {
            if (*this)
                TRAITS::Close(value);
        }
    };

    template <typename TRAITS> void swap(unique_handle<TRAITS>& left, unique_handle<TRAITS>& right) noexcept {
        left.swap(right);
    }

    template <typename TRAITS> bool operator<=>(unique_handle<TRAITS> const& left, unique_handle<TRAITS> const& right) {
        return left.Get() <=> right.Get();
    }


}
