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
#include "FileService.h"

namespace filesystem = std::filesystem;
using std::vector;

namespace Shared::Service {

    vector<filesystem::path> FileService::GetFilesFromDirectory(filesystem::path const& folder, std::wregex const& filter) const noexcept {
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

    bool FileService::DirectoryExists(std::string_view const path) const {
        filesystem::path const folder(path);
        return filesystem::exists(folder) && filesystem::is_directory(folder);
    }

}