#pragma once

#include "Common.h"
#include <filesystem>
#include <optional>
#include <string>

namespace SymbolPath::Model
{
    class SettingsImpl;

    class Settings final
    {
    public:
        SYMBOL_PATH_DLL explicit Settings(); 
        SYMBOL_PATH_DLL Settings(Settings const& other);
        SYMBOL_PATH_DLL Settings(Settings&& other) noexcept = default;
        SYMBOL_PATH_DLL ~Settings() = default;

        SYMBOL_PATH_DLL Settings& operator=(Settings const& other);
        SYMBOL_PATH_DLL Settings& operator=(Settings&&) noexcept = default;

        [[nodiscard]] SYMBOL_PATH_DLL std::filesystem::path GetLocalCache() const noexcept;
        SYMBOL_PATH_DLL void SetLocalCache(std::filesystem::path value) noexcept;
        [[nodiscard]] SYMBOL_PATH_DLL std::optional<std::string> GetSymbolPath() const noexcept;
        SYMBOL_PATH_DLL void SetSymbolPath(std::string value) noexcept;

    private:
        std::unique_ptr<SettingsImpl> pImpl;

    };
}

