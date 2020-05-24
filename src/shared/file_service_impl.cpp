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
#include "file_service_impl.h"

using std::vector;

namespace shared::service
{

shared_file_service make_file_service()
{
    return std::make_shared<file_service_impl>();
}
shared_const_file_service make_const_file_service()
{
    return std::make_shared<file_service_impl const>();
}

unique_file_service make_unique_file_service()
{
    return std::make_unique<file_service_impl>();
}
unique_const_file_service make_unique_const_file_service()
{
    return std::make_unique<file_service_impl const>();
}

vector<std::filesystem::path> file_service_impl::get_files_from_directory(std::filesystem::path const& folder, std::wregex const& filter) const noexcept
{
    try {
        if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder))
            return vector<std::filesystem::path>();

        vector<std::filesystem::path> matches;
        auto const files = std::filesystem::directory_iterator(folder);
        copy_if(begin(files), end(files), back_inserter(matches),
            [&filter](auto const& entry) {
                return entry.is_regular_file() && regex_match(entry.path().filename().wstring(), filter);
            });

        return matches;
    }
    catch (std::exception const&) {
        return vector<std::filesystem::path>();
    }
}

bool file_service_impl::directory_exists(std::string_view const path) const
{
    std::filesystem::path const folder(path);
    return std::filesystem::exists(folder) && std::filesystem::is_directory(folder);
}

}