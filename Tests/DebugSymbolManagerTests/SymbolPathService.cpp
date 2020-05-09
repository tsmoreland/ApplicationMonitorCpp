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

using std::filesystem::path;
using std::move;
using std::optional;
using std::string;
using std::swap;
using std::unique_ptr;
using std::vector;
using std::wregex;

#define BOOST_TEST_MODULE SymbolPathService
#include <boost/test/included/unit_test.hpp>

#include "TestAdapter.h"
#include "TestFixture.h"

using DebugSymbolManager::Model::Settings;
using DebugSymbolManager::Service::ISymbolPathService;
using DebugSymbolManager::Service::SymbolPathService;

#include "MockObjects.h"

#pragma warning(push)
#pragma warning(disable:4455)
using std::literals::string_literals::operator ""s;
#pragma warning(pop)

namespace {
    constexpr auto SYMBOL_PATH_VAR = "_NT_SYMBOL_PATH";
    constexpr auto SYMBOL_SERVER = "*SRV";
    
    struct ExpectedSetCall {
        Cardinality Cardinality;
        string Value;
        bool Success;
    };

    struct Context {
        Context() {
            EnviromentRepository = make_unique<MockObjects::MockEnviromentRepository>();
            FileService = make_unique<MockObjects::MockFileService>();
        }
        Context(Context const&) = delete;
        Context(Context&& other) noexcept
            : EnviromentRepository(other.EnviromentRepository.release())
            , FileService(other.FileService.release())
            , ApplicationPath(move(other.ApplicationPath))
            , InitialSymbolPath(move(other.InitialSymbolPath))
            , ExpectedSymbolPath(move(other.ExpectedSymbolPath))
            , NumberOfGetCalls(move(other.NumberOfGetCalls))
            , NumberOfInitialSetCalls(move(other.NumberOfInitialSetCalls))
            , NumberOfSetCalls(move(other.NumberOfSetCalls))
            , InitialSetReturns(other.InitialSetReturns)
            , SetReturns(other.SetReturns) {
            other.Reset();
        }
        Context& operator=(Context const& other) = delete;
        Context& operator=(Context&& other) noexcept {
            if (this == &other)
                return *this;

            swap(EnviromentRepository, other.EnviromentRepository);
            swap(FileService, other.FileService);
            swap(ApplicationPath, other.ApplicationPath);
            swap(InitialSymbolPath, other.InitialSymbolPath);
            swap(ExpectedSymbolPath, other.ExpectedSymbolPath);
            swap(NumberOfGetCalls, other.NumberOfGetCalls);
            swap(NumberOfInitialSetCalls, other.NumberOfInitialSetCalls);
            swap(NumberOfSetCalls, other.NumberOfSetCalls);
            swap(InitialSetReturns, other.InitialSetReturns);
            swap(SetReturns, other.SetReturns); 

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
        }

        unique_ptr<MockObjects::MockEnviromentRepository> EnviromentRepository{};
        unique_ptr<MockObjects::MockFileService> FileService{};
        optional<SymbolPathService> Service{};

        Settings Settings{SYMBOL_SERVER};
        std::string ApplicationPath{};
        std::string InitialSymbolPath{};
        std::string ExpectedSymbolPath{};
        Cardinality NumberOfGetCalls{AnyNumber()};
        vector<ExpectedSetCall> ExpectedSetCalls;
    };

    class ContextBuilder {
        Context m_context;

        template <typename UPDATER>
        ContextBuilder& UpdateObject(UPDATER updater) {
            updater(m_context);
            return *this;
        }
    public:
        static ContextBuilder ArrangeForConstructorTest(string const& initialVariableValue) {
            ContextBuilder builder;
            builder
                .WithInitialVariable(initialVariableValue)
                .WithGetCalledCountTimes(Exactly(1));
            return builder;
        }
        static ContextBuilder Arrange() {
            ContextBuilder builder;
            builder
                .WithInitialVariable(SYMBOL_SERVER);
            return builder;
        }

        Context&& Build() {
            if (!m_context.InitialSymbolPath.empty()) {
                EXPECT_CALL(*m_context.EnviromentRepository, GetVariable(SYMBOL_PATH_VAR))
                    .Times(m_context.NumberOfGetCalls)
                    .WillOnce(Return(optional(m_context.InitialSymbolPath)));
            }
            for (auto& expected : m_context.ExpectedSetCalls)
                EXPECT_CALL(*m_context.EnviromentRepository, SetVariable(SYMBOL_PATH_VAR, expected.Value))
                    .Times(expected.Cardinality)
                    .WillRepeatedly(Return(expected.Success));

            return move(m_context);
        }

        ContextBuilder& WithInitialVariable(string symbolPath) {
            return UpdateObject(
                [&symbolPath](Context& context) {
                    context.InitialSymbolPath = move(symbolPath);
                });
        }
        ContextBuilder& WithExpectedVariable(string symbolPath) {
            return UpdateObject(
                [&symbolPath](Context& context) {
                    context.ExpectedSymbolPath = move(symbolPath);
                });
        }
        ContextBuilder& WithApplicationPath(string applicationPath) {
            return UpdateObject(
                [&applicationPath](auto& context) {
                    context.ApplicationPath = move(applicationPath);
                });
        }
        ContextBuilder& WithGetCalledCountTimes(Cardinality count) {
            return UpdateObject(
                [&count](auto& context) {
                    context.NumberOfGetCalls = move(count);
                });
        }
        ContextBuilder& WithInitialSetCalledCountTimes(Cardinality count) {
            return UpdateObject(
                [&count](auto& context) {
                    context.NumberOfInitialSetCalls = move(count);
                });
        }
        ContextBuilder& WithSetCalledCountTimes(Cardinality count) {
            return UpdateObject(
                [&count](auto& context) {
                    context.NumberOfSetCalls = move(count);
                });
        }
        ContextBuilder& WithInitialSetReturns(bool success) {
            return UpdateObject(
                [success](auto& context) {
                    context.InitialSetReturns = success;
                });
        }
        ContextBuilder& WithSetReturns(bool success) {
            return UpdateObject(
                [success](auto& context) {
                    context.SetReturns = success;
                });
        }
    };

}


BOOST_AUTO_TEST_CASE(ConstructorGetsCurrentSymbolPath) {
    // Arrange
    auto const context = ContextBuilder::ArrangeForConstructorTest("symPath123").Build();

    // Act
    SymbolPathService service(context.Settings, *context.EnviromentRepository, *context.FileService);
    
    // Assert -- handled by setup in arrange (EXPECT_CALL)
}


BOOST_AUTO_TEST_CASE(ConstructorUpdatesCurrentSymbolPathWhenHasValue) {
    // Arrange
    auto const context = ContextBuilder::ArrangeForConstructorTest("symPath123")
        .WithInitialSetCalledCountTimes(Exactly(1))
        .WithInitialSetReturns(true)
        .Build();

    // Act
    SymbolPathService service(context.Settings, *context.EnviromentRepository, *context.FileService);
    
    // Assert -- covered by expect call
}

BOOST_AUTO_TEST_CASE(UpdateApplicationPathChangesSymbolPath) {
    // Arrange
    auto const appPath = R"(C:\Program Files\Application)"s;
    auto context = ContextBuilder::Arrange()
        .WithExpectedVariable(string(SYMBOL_SERVER) + ";"s + appPath)
        .WithSetCalledCountTimes(Exactly(1))
        .Build();

    // Act
    auto const result = context.Service.value().UpdateApplicationPath(appPath);

    // Assert

}
