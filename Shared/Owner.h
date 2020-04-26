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

#include <memory>
#include "BadOwnerAccess.h"

namespace Shared::Infrastructure {

    /// <summary>Decorator of unique_ptr<TINTERFACE> combining it with Optional</summary>
    template<typename TRAITS>
    class Owner final {
        using SmartPointer = TRAITS::SmartPointer;
        using ValueType = TRAITS::ValueType;
    public:
        [[nodiscard]] bool HasValue() const noexcept {
            return static_cast<bool>(owned);
        }

        [[nodiscard]] ValueType& Value() {
            return HasValue() ? *owned.get() : throw BadOwnerAccess();
        }
        [[nodiscard]] ValueType& Value() const {
            return HasValue() ? *owned.get() : throw BadOwnerAccess();
        }
        [[nodiscard]] ValueType& OrElse(ValueType& other) noexcept {
            return owned ? *owned.get() : other;
        }
        template<typename SUPPLIER>
        [[nodiscard]] ValueType& OrElse(SUPPLIER other) {
            return owned ? *owned.get() : SUPPLIER();
        }

        ValueType* operator->() {
            return owned ? owned.get() : throw BadOwnerAccess();
        }
        ValueType* const* operator->() const {
            return owned ? owned.get() : throw BadOwnerAccess();
        }

        void swap(SmartPointer& other) noexcept {
            std::swap(owned, other.owned);
        }

        explicit Owner() = default;
        explicit Owner(SmartPointer owned) {
            owned = TRAITS ::Build(owned);
        }
        explicit Owner(ValueType&& owned) {
            owned = TRAITS::Build(move(owned));
        }

        static constexpr Owner<TRAITS> Empty() {
            return Owner<TRAITS>();
        };
    private:
        SmartPointer owned;
    };

}
