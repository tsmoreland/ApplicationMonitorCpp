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

    template<typename TINTERFACE>
    struct UniqueOwnerTraits {
        using SmartPointer = std::unique_ptr<TINTERFACE>;
        using ValueType = TINTERFACE;

        constexpr static SmartPointer Build(ValueType&& value) {
            return make_unique<TINTERFACE>(std::forward(value));
        }
        constexpr static SmartPointer Build(std::unique_ptr<TINTERFACE> value) {
            return move(value);
        }
    };


    /// <summary>Decorator of unique_ptr<TINTERFACE> combining it with Optional</summary>
    template<typename TINTERFACE>
    class UniqueOwner final {
    public:
        explicit UniqueOwner() = default;
        explicit UniqueOwner(std::unique_ptr<TINTERFACE> owned) : owned(move(owned)) {
        }
        explicit UniqueOwner(TINTERFACE&& owned) {
            owned = make_unique<TINTERFACE>(std::forward(owned));
        }

        [[nodiscard]] bool HasValue() const noexcept {
            return static_cast<bool>(owned);
        }

        [[nodiscard]] TINTERFACE& Value() {
            return HasValue() ? *owned.get() : throw BadOwnerAccess();
        }
        [[nodiscard]] TINTERFACE& Value() const {
            return HasValue() ? *owned.get() : throw BadOwnerAccess();
        }
        [[nodiscard]] TINTERFACE& OrElse(TINTERFACE& other) noexcept {
            return owned ? *owned.get() : other;
        }
        template<typename SUPPLIER>
        [[nodiscard]] TINTERFACE& OrElse(SUPPLIER other) {
            return owned ? *owned.get() : SUPPLIER();
        }

        TINTERFACE* operator->() {
            return owned ? owned.get() : throw BadOwnerAccess();
        }
        TINTERFACE const* operator->() const {
            return owned ? owned.get() : throw BadOwnerAccess();
        }

        void swap(UniqueOwner<TINTERFACE>& other) noexcept {
            std::swap(owned, other.owned);
        }

        static constexpr UniqueOwner<TINTERFACE> Empty() {
            return UniqueOwner<TINTERFACE>();
        };
    private:
        std::unique_ptr<TINTERFACE> owned;
    };


}
