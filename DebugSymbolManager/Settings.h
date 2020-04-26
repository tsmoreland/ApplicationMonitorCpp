#pragma once

#include <string>
#include <DataMember.h>
#include <string_extensions.h>

namespace DebugSymbolManager::Model {
    class SettingsImpl;

    struct Settings final {
        std::string LocalCache;
        std::string BaseSymbolPath;
        constexpr static auto LOCAL_CACHE_KEY = "[Local Cache]";
        constexpr static auto LOCAL_CACHE_KEY_SIZE = extension::zstring_length("[Local Cache]");

        bool operator==(Settings const& other) const noexcept {
            return Equals(other);
        }
        bool operator!=(Settings const& other) const noexcept {
            return !Equals(other);
        }
        [[nodiscard]] bool Equals(Settings const& other) const noexcept {
            return this == &other ||
                BaseSymbolPath == other.BaseSymbolPath &&
                LocalCache == other.BaseSymbolPath;
        }

        constexpr static auto PROPERTIES = std::make_tuple(
            Shared::Infrastructure::property(&Settings::LocalCache, "localCache"),
            Shared::Infrastructure::property(&Settings::BaseSymbolPath, "baseSymbolPath")
        );
    };

    inline bool operator==(Settings const& left, Settings const& right) {
        return left.Equals(right);
    }
    inline bool operator!=(Settings const& left, Settings const& right) {
        return !left.Equals(right);
    }

}

