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
#include "Settings.h"
#include "SettingsImpl.h"
#include <sstream>

using std::filesystem::path;
using std::find;
using std::make_unique;
using std::move;
using std::nullopt;
using std::optional;
using std::stringstream;
using std::string;
using std::unique_ptr;
using std::vector;

namespace SymbolPath::Model
{
    Settings::Settings()
    {
        pImpl = make_unique<SettingsImpl>();
    }
    Settings::Settings(Settings const& other)
    {
        if (this == &other)
            return;

        if (!pImpl)
            pImpl = make_unique<SettingsImpl>();
        *pImpl = *other.pImpl;
    }
    Settings& Settings::operator=(Settings const& other)
    {
        if (this == &other)
            return *this;

        if (!pImpl)
            pImpl = make_unique<SettingsImpl>();
        *pImpl = *other.pImpl;
        return *this;
    }
    path Settings::GetLocalCache() const noexcept
    {
        return pImpl ? pImpl->GetLocalCache() : path();
    }

    void Settings::SetLocalCache(std::filesystem::path value) noexcept
    {
        if (pImpl)
            pImpl->SetLocalCache(move(value));
    }

    optional<string> Settings::GetSymbolPath() const noexcept
    {
        if (pImpl)
            return pImpl->GetSymbolPath();
        else
            return nullopt;
    }

    void Settings::SetSymbolPath(std::string value) noexcept
    {
        if (pImpl)
            pImpl->SetSymbolPath(move(value));
    }

}
