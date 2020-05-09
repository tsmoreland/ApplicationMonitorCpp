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

#include "MockObjects.h"
#include "ExpectedSetCall.h"

namespace DebugSymbolManager::Test {
    constexpr auto SYMBOL_PATH_VAR = "_NT_SYMBOL_PATH";
    constexpr auto SYMBOL_SERVER = "*SRV";

    struct Context {
        Context() {
            EnviromentRepository = make_unique<MockObjects::MockEnviromentRepository>();
            FileService = make_unique<MockObjects::MockFileService>();
        }
        Context(Context const&) = delete;
        Context(Context&& other) noexcept
            : EnviromentRepository(other.EnviromentRepository.release())
            , FileService(other.FileService.release())
            , Service(other.Service.release())
            , ApplicationPath(move(other.ApplicationPath))
            , InitialSymbolPath(move(other.InitialSymbolPath))
            , ExpectedSymbolPath(move(other.ExpectedSymbolPath))
            , NumberOfGetCalls(move(other.NumberOfGetCalls))
            , ExpectedSetCalls(move(other.ExpectedSetCalls)) 
            , ExistingDirectories(move(other.ExistingDirectories)) {
            other.Reset();
        }
        Context& operator=(Context const& other) = delete;
        Context& operator=(Context&& other) noexcept {
            if (this == &other)
                return *this;

            ::swap(EnviromentRepository, other.EnviromentRepository);
            ::swap(FileService, other.FileService);
            ::swap(ApplicationPath, other.ApplicationPath);
            ::swap(InitialSymbolPath, other.InitialSymbolPath);
            ::swap(ExpectedSymbolPath, other.ExpectedSymbolPath);
            ::swap(NumberOfGetCalls, other.NumberOfGetCalls);
            ::swap(ExpectedSetCalls, other.ExpectedSetCalls);
            ::swap(Service, other.Service);
            ::swap(ExistingDirectories, other.ExistingDirectories);

            other.Reset();
            return *this;
        }
        ~Context() = default;

        void Reset() noexcept {
            EnviromentRepository = make_unique<MockObjects::MockEnviromentRepository>();
            FileService = make_unique<MockObjects::MockFileService>();
            Settings = ::Settings{SYMBOL_SERVER};
            NumberOfGetCalls = AnyNumber();
            ExpectedSetCalls.clear();
            ExistingDirectories.clear();
        }

        unique_ptr<MockObjects::MockEnviromentRepository> EnviromentRepository{};
        unique_ptr<MockObjects::MockFileService> FileService{};
        unique_ptr<SymbolPathService> Service{};

        Settings Settings{SYMBOL_SERVER};
        std::string ApplicationPath{};
        std::string InitialSymbolPath{};
        std::string ExpectedSymbolPath{};
        Cardinality NumberOfGetCalls{AnyNumber()};
        vector<ExpectedSetCall> ExpectedSetCalls{};
        vector<string> ExistingDirectories{};
    };

}
