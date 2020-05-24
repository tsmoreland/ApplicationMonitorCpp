#pragma once

#include <string>
#include <data_member.h>

namespace debug_symbol_manager::model
{

    struct settings final
    {
        std::string base_symbol_path;

        bool operator==(settings const& other) const noexcept
        {
            return equals(other);
        }
        bool operator!=(settings const& other) const noexcept
        {
            return !equals(other);
        }
        [[nodiscard]] bool equals(settings const& other) const noexcept
        {
            return this == &other ||
                base_symbol_path == other.base_symbol_path;
        }

        constexpr static auto PROPERTIES = std::make_tuple(
            shared::infrastructure::property(&settings::base_symbol_path, "baseSymbolPath")
        );
    };

    inline bool operator==(settings const& left, settings const& right)
    {
        return left.equals(right);
    }
    inline bool operator!=(settings const& left, settings const& right)
    {
        return !left.equals(right);
    }

}

