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
#include "NtSymbolPath.h"
#include <sstream>

using std::filesystem::path;
using std::find;
using std::move;
using std::nullopt;
using std::optional;
using std::string;
using std::stringstream;
using std::string_view;

using Shared::Model::CommandResult;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator""s;
#pragma warning(pop)

namespace DebugSymbolManager::Model {

    NtSymbolPath::NtSymbolPath(Shared::Service::IFileService const& fileService)
        : m_fileService(fileService) {

    }

    optional<string> NtSymbolPath::GetSymbolPath() const noexcept {
        try {
            stringstream builder{};

            if (m_baseSymbolPath != ""s)
                builder << m_baseSymbolPath;

            for (auto const& path : m_additionalPaths)
                builder << ";" << path;

            return optional(builder.str());
        }
        catch (std::exception const&) {
            return nullopt;
        }
    }

    std::string const& NtSymbolPath::GetBaseSymbolPath() const noexcept
    {
        return m_baseSymbolPath;
    }

    void NtSymbolPath::SetBaseSymbolPath(std::string server) noexcept {
        if (server == m_baseSymbolPath)
            return;
        m_baseSymbolPath = move(server);
        UpdateIsModified();
    }

    CommandResult NtSymbolPath::AddDirectory(std::string const& directory) noexcept {
        try {
            if (directory.empty() || !m_fileService.DirectoryExists(directory))
                return CommandResult::Fail("Directory not found");

            if (find(begin(m_additionalPaths), end(m_additionalPaths), directory) == end(m_additionalPaths))
                return CommandResult::Ok("Already present");

            m_additionalPaths.emplace_back(directory);
            UpdateIsModified();
            return CommandResult::Ok();

        } catch (std::exception const& ex) {
            return CommandResult::Error(ex);
        }
    }

    void NtSymbolPath::RemoveDirectory(std::string const& directory) noexcept {
        auto const& entry = find(begin(m_additionalPaths), end(m_additionalPaths), directory);
        if (entry == end(m_additionalPaths))
            return;
        m_additionalPaths.erase(entry);
        UpdateIsModified();
    }

    bool NtSymbolPath::IsModified() const noexcept {
        return m_isModified;
    }

    CommandResult NtSymbolPath::Reset(string const& currentValue) noexcept {
        try {
            m_lastSavedState = currentValue;
            UpdateIsModified();
            return CommandResult::Ok();

        } catch (std::exception const& ex) {
            return CommandResult::Error(ex);
        }
    }
    void NtSymbolPath::UpdateIsModified() noexcept {
        m_isModified = m_lastSavedState != GetSymbolPath().value_or(""s) || m_lastSavedState == ""s;
    }


}