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

#include "Context.h"

namespace DebugSymbolManager::Test {

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

            for (auto const& directory : m_context.ExistingDirectories)
                EXPECT_CALL(*m_context.FileService, DirectoryExists(string_view(directory)))
                    .WillRepeatedly(Return(true));

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
        template<typename TExpectedSetCall, typename... Args> // could be done with std::initializer_list but I wanted an excuse for varadic template
        ContextBuilder& WithExpectedSetCalls(TExpectedSetCall first, Args... expectedCalls) {
            // duck typing would prevent issues here but it's an excuse to use static assert and in this scenario ExpectedSetCall is the only valid type
            static_assert(std::is_same<TExpectedSetCall, ExpectedSetCall>::value, "invalid type, must use ExpectedSetCall");
            m_context.ExpectedSetCalls.push_back(move(first));
            return WithExpectedSetCalls(expectedCalls...);
        }
        template <typename TExpectedSetCall>
        ContextBuilder& WithExpectedSetCalls(TExpectedSetCall expectedCall) {
            m_context.ExpectedSetCalls.push_back(move(expectedCall));
            return *this;
        }
        ContextBuilder& WithExistingDirectories(std::initializer_list<string> directories) {
            return UpdateObject(
                [&directories](Context& context) {
                    copy(directories.begin(), directories.end(), 
                        back_inserter(context.ExistingDirectories));
                });
        }
        ContextBuilder& WithServiceCreated() {
            return UpdateObject(
                [](Context& context) {
                    auto service = make_unique<SymbolPathService>(context.Settings, *context.EnviromentRepository, *context.FileService);
                    context.Service  = move(service);
                });
        }
    };
}
