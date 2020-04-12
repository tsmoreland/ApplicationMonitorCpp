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

namespace Shared::Win32
{
    template <typename Traits>
    class UniqueHandle final
    {
        using Pointer = typename Traits::Pointer;
    public:
        [[nodiscard]] Pointer Get() const noexcept
        {
            return _value;
        }
        [[maybe_unused]] Pointer Release() noexcept
        {
            auto value = _value;
            _value = Traits::Invalid();
            return value;
        }
        [[maybe_unused]] bool Reset(Pointer value = Traits::Invalid()) noexcept
        {
            if (_value == value)
                return static_cast<bool>(*this);

            Close();
            _value = value;
            return static_cast<bool>(*this);
        }
        explicit operator bool() const noexcept
        {
            return _value != Traits::Invalid();
        }

        explicit UniqueHandle(Pointer value = Traits::Invalid()) noexcept
            : _value{value}
        {
        }
        UniqueHandle(const UniqueHandle&) = delete;
        UniqueHandle& operator=(const UniqueHandle&) = delete;
        UniqueHandle(UniqueHandle&& other) noexcept
            : _value{std::move(other.Release())}
        {
        }
        UniqueHandle& operator=(UniqueHandle&& other) noexcept
        {
            if (this != &other)
                reset(other.Release());
            return *this;
        }
        ~UniqueHandle()
        {
            Close();
        }
        void swap(UniqueHandle<Traits>& other) noexcept
        {
            std::swap(_value, other._value);
        }

    private:
        Pointer _value;

        void Close() noexcept
        {
            if (*this)
                Traits::Close(_value);
        }
    };

    template <typename Traits> void swap(UniqueHandle<Traits>& left, UniqueHandle<Traits>& right) noexcept
    {
        left.swap(right);
    }

    template <typename Traits> bool operator<=>(UniqueHandle<Traits> const& left, UniqueHandle<Traits> const& right)
    {
        return left.Get() <=> right.Get();
    }


}
