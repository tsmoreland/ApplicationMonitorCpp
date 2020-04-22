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
#include "IEnvironmentService.h"
#include "EnvironmentService.h"
#include "Process.h"

namespace filesystem = std::filesystem;

using std::back_inserter;
using std::copy_if;
using std::nullopt;
using std::optional;
using std::string;
using std::string_view;
using std::unique_ptr;
using std::vector;

using Shared::Model::IProcess;
using Shared::Model::Process;

namespace Shared::Services
{
    optional<unique_ptr<IProcess>> EnvironmentService::StartProcess(string_view const& filename, string_view const& arguments) const noexcept
    {
        try
        {
            return optional<unique_ptr<IProcess>>(Process::Start(filename, arguments));
        }
        catch (const std::exception&)
        {
            return nullopt;
        }
    }

    vector<unique_ptr<IProcess>> EnvironmentService::GetProcessesByName(string_view const& processName) const noexcept
    {
        try
        {
            return Process::GetProcessesByName(processName);
        }
        catch (const std::exception&)
        {
            return vector<unique_ptr<IProcess>>();
        }
    }
    std::optional<std::string> EnvironmentService::GetVariable(std::string const& key) const noexcept
    {
        char value[4096]{};
        if (GetEnvironmentVariableA(key.c_str(), value, 16384) == FALSE)
            return nullopt;
        return optional(string(value));
    }
    bool EnvironmentService::SetVariable(std::string const& key, std::string const& value) const noexcept
    {
        return SetEnvironmentVariableA(key.c_str(), value.c_str()) == TRUE;
    }
    vector<filesystem::path> EnvironmentService::GetFilesFromDirectory(filesystem::path& folder, string_view const& filter) const noexcept
    {
        try
        {
            if (!filesystem::exists(folder) || !filesystem::is_directory(folder))
                return vector<filesystem::path>();

            vector<filesystem::path> matches{};
            auto const files = filesystem::directory_iterator(folder);
            copy_if(begin(files), end(files), back_inserter(matches), [&matches](filesystem::directory_entry const& entry)
            {
                if (!entry.is_regular_file())
                    return false;
                auto const& filename = entry.path().filename().c_str();



                return true;
            });

            return matches;
        }
        catch (std::exception const&)
        {
            return vector<filesystem::path>();
        }
    }
}
