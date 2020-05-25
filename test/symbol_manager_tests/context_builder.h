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

#include "test_context.h"

namespace symbol_manager::test
{
    class context_builder
    {
        test_context m_context;

        template <typename UPDATER>
        context_builder& update_object(UPDATER updater)
        {
            updater(m_context);
            return *this;
        }
    public:
        static context_builder arrange_for_constructor_test(string const& initial_variable_value)
        {
            context_builder builder;
            builder
                .with_initial_variable(initial_variable_value)
                .with_get_called_count_times(Exactly(1));
            return builder;
        }
        static context_builder arrange()
        {
            context_builder builder;
            builder
                .with_initial_variable(SYMBOL_SERVER);
            return builder;
        }

        test_context&& Build()
        {
            if (!m_context.initial_symbol_path.empty()) {
                EXPECT_CALL(*m_context.repository, get_variable(SYMBOL_PATH_VAR))
                    .Times(m_context.number_of_get_calls)
                    .WillOnce(Return(optional(m_context.initial_symbol_path)));
            }
            for (auto& expected : m_context.expected_set_calls)
                EXPECT_CALL(*m_context.repository, set_variable(SYMBOL_PATH_VAR, expected.value))
                    .Times(expected.cardinality)
                    .WillRepeatedly(Return(expected.success));

            for (auto const& directory : m_context.existing_directories)
                EXPECT_CALL(*m_context.file_service, directory_exists(string_view(directory)))
                    .WillRepeatedly(Return(true));

            return move(m_context);
        }

        context_builder& with_initial_variable(string symbolPath)
        {
            return update_object(
                [&symbolPath](auto& context) {
                    context.initial_symbol_path = move(symbolPath);
                });
        }
        context_builder& with_expected_variable(string symbolPath)
        {
            return update_object(
                [&symbolPath](auto& context) {
                    context.expected_symbol_path = move(symbolPath);
                });
        }
        context_builder& with_application_path(string application_path)
        {
            return update_object(
                [&application_path](auto& context) {
                    context.application_path = move(application_path);
                });
        }
        context_builder& with_get_called_count_times(Cardinality count)
        {
            return update_object(
                [&count](auto& context) {
                    context.number_of_get_calls = move(count);
                });
        }
        template<typename TExpectedSetCall, typename... Args> // could be done with std::initializer_list but I wanted an excuse for varadic template
        context_builder& with_expected_set_calls(TExpectedSetCall first, Args... expected_calls)
        {
            // duck typing would prevent issues here but it's an excuse to use static assert and in this scenario ExpectedSetCall is the only valid type
            static_assert(std::is_same<TExpectedSetCall, expected_set_call>::value, "invalid type, must use ExpectedSetCall");
            m_context.expected_set_calls.push_back(move(first));
            return with_expected_set_calls(expected_calls...);
        }
        template <typename TExpectedSetCall>
        context_builder& with_expected_set_calls(TExpectedSetCall expected_call)
        {
            m_context.expected_set_calls.push_back(move(expected_call));
            return *this;
        }
        context_builder& with_existing_directories(std::initializer_list<string> directories)
        {
            return update_object(
                [&directories](auto& context) {
                    copy(directories.begin(), directories.end(), 
                        back_inserter(context.existing_directories));
                });
        }
        context_builder& with_service_created()
        {
            return update_object(
                [](test_context& context) {
                    auto service = make_unique<symbol_manager::service::symbol_path_service_impl>(context.settings, context.repository, context.file_service);
                    context.service  = move(service);
                });
        }
    };
}
