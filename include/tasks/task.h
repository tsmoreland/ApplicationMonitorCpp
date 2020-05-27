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

#include <memory>
#include <optional>
#include <tasks/task_base.h>
#include <tasks/task_action.h>
#include <shared/future.h>

#pragma warning(push)
#pragma warning(disable:4455)
using std::chrono_literals::operator ""ms;
#pragma warning(pop)

namespace tasks
{

    using future_state = std::future<task_state>;

    template <typename TASK_ACTION>
    class task final : public task_base
    {
    public:
        explicit task()
            : task_base()
            , m_action(this)
        {
        }

        std::optional<std::chrono::milliseconds> process() final
        {
            if (!has_pending_state(m_pending_state)) {
                auto [state, remaining] = m_action.process_async(get_current_state());
                m_pending_state = state;
                return remaining;
            }
            auto [maybe_state, remaining] = process_pending_state();
            if (!maybe_state.has_value()) 
                return remaining;

            update_task_state(maybe_state.value());
            m_pending_state = std::nullopt;

            return 0ms;
        }

    private:
        TASK_ACTION m_action{};
        std::optional<future_state> m_pending_state{};

        [[nodiscard]] static bool has_pending_state(std::optional<future_state> const& state)
        {
            return state != std::nullopt;
        }
        std::pair<std::optional<task_state>, std::chrono::milliseconds> process_pending_state()
        {
            return extension::is_ready(m_pending_state.value()
                ? std::make_pair(std::optional(m_pending_state.value().get()), 0ms)
                : std::make_pair(std::nullopt, m_action.get_time_remiaining()));
        }
    };

}
