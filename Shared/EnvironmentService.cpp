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
#include "IProcessService.h"
#include "IFileService.h"
#include "EnvironmentService.h"
#include "ProcessImpl.h"

namespace filesystem = std::filesystem;

using std::back_inserter;
using std::copy_if;
using std::make_tuple;
using std::move;
using std::nullopt;
using std::optional;
using std::regex_match;
using std::string;
using std::string_view;
using std::transform;
using std::tuple;
using std::unique_ptr;
using std::vector;
using std::wregex;
using std::wstring;
using std::wstring_view;

using extension::string_equal;
using extension::string_split;
using extension::string_contains_in_order;

using Shared::Infrastructure::UniqueOwner;
using Shared::Model::IProcess;
using Shared::Model::ProcessImpl;
using Process = Shared::Infrastructure::UniqueOwner<Shared::Model::IProcess>;

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
#pragma warning(pop)

namespace Shared::Services {

    Process EnvironmentService::StartProcess(string_view const& filename, string_view const& arguments) const noexcept
    {
        try {
            return Process(ProcessImpl::Start(filename, arguments));
        }
        catch (const std::exception&) {
            return UniqueOwner<IProcess>::Empty();
        }
    }
    vector<Process> EnvironmentService::GetProcessesByName(string_view const& processName) const noexcept {
        try {
            const size_t minimumSize = 100;
            auto processImplementations = ProcessImpl::GetProcessesByName(processName);
            vector<Process> processes{};
            processes.reserve(minimumSize);

            transform(begin(processImplementations), end(processImplementations), back_inserter(processes),
                [](auto& pImpl) {
                    return UniqueOwner<IProcess>(move(pImpl));
                });

            return processes;
        }
        catch (std::exception const&) {
            return vector<Process>();
        }
    }
    optional<string> EnvironmentService::GetVariable(std::string const& key) const noexcept {
        constexpr auto MAX_VARIABLE_NAME_SIZE = 8192;
        char value[MAX_VARIABLE_NAME_SIZE]{};
        if (GetEnvironmentVariableA(key.c_str(), value, MAX_VARIABLE_NAME_SIZE) == FALSE)
            return nullopt;
        return optional(string(value));
    }
    bool EnvironmentService::SetVariable(string const& key, string const& value) const noexcept {
        return SetEnvironmentVariableA(key.c_str(), value.c_str()) == TRUE;
    }
    vector<filesystem::path> EnvironmentService::GetFilesFromDirectory(filesystem::path const& folder, std::wregex const& filter) const noexcept {
        try {
            if (!filesystem::exists(folder) || !filesystem::is_directory(folder))
                return vector<filesystem::path>();

            vector<filesystem::path> matches;
            auto const files = filesystem::directory_iterator(folder);
            copy_if(begin(files), end(files), back_inserter(matches),
                [&filter](auto const& entry) {
                    return entry.is_regular_file() && regex_match(entry.path().filename().wstring(), filter);
                });

            return matches;
        }
        catch (std::exception const&) {
            return vector<filesystem::path>();
        }
    }

    optional<filesystem::path> EnvironmentService::GetPathToRunningProcess(string_view const& processName) const noexcept {
        return ProcessImpl().GetPathToRunningProcess(processName);
    }

    bool EnvironmentService::DirectoryExists(std::string_view const path) const {
        filesystem::path const folder(path);
        return filesystem::exists(folder) && filesystem::is_directory(folder);
    }

}
