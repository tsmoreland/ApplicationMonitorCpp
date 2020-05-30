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

#pragma warning(push)
#pragma warning(disable:4455)
using std::chrono_literals::operator ""ms;
#pragma warning(pop)

namespace tasks
{
    template <class TASK_STATE_BASE>
    using unique_task_state = std::unique_ptr<TASK_STATE_BASE>;

    template <class TASK_STATE_BASE>
    class task final : public task_base
    {
    public:
        task()
            : task(TASK_STATE_BASE::initialize())
        {
        }
        explicit task(unique_task_state<TASK_STATE_BASE> base_state)
            : task_base()
            , m_state(base_state.release())
        {
        }
        task(task&& other) noexcept
            : m_state(other.m_state.release())
        {
        }
        task(task const&) = delete;
        virtual ~task()
        {
            m_state.reset();
        }

        void process() final
        {
            auto next_state = m_state->get_next_state();
            m_state = std::move(next_state);

            update_time_remaining(m_state->get_time_remaining().value_or(0ms));
            update_task_status(m_state->get_task_status());
        }
        [[nodiscard]] bool equals(task const& other) const
        {
            return
                m_state.get() == other.m_state.get() || 
                m_state != nullptr && *m_state == *other.m_state;
        }

        void swap(task& other) noexcept
        {
            std::swap(m_state, other.m_state);
        }

        task& operator=(task&& other) noexcept
        {
            std::swap(m_state, other.m_state); 
            return *this;
        }
        task& operator=(task const&) = delete;
        [[nodiscard]] bool operator==(task const& other) const
        {
            return equals(other);
        }
        [[nodiscard]] bool operator!=(task const& other) const
        {
            return !(*this == other);
        }
        friend bool operator==(task const& left, task const& right);
        friend bool operator!=(task const& left, task const& right);
    private:
        unique_task_state<TASK_STATE_BASE> m_state;
    };

    template <class TASK_STATE_BASE>
    [[nodiscard]] bool operator==(task<TASK_STATE_BASE> const& left, task<TASK_STATE_BASE> const& right)
    {
        return left.equals(right);
    }

    template <class TASK_STATE_BASE>
    [[nodiscard]] bool operator!=(task<TASK_STATE_BASE> const& left, task<TASK_STATE_BASE> const& right)
    {
        return !left.equals(right);
    }

    template <class TASK_STATE_BASE>
    void swap(task<TASK_STATE_BASE>& left, task<TASK_STATE_BASE>& right) noexcept
    {
        left.swap(right);
    }

    template <class TASK_STATE_BASE, class... ARGUMENTS>
    unique_task_state<TASK_STATE_BASE> make_unique_task(ARGUMENTS&&... args)
    {
        return std::make_unique<task<TASK_STATE_BASE>>(std::forward<ARGUMENTS>(args)...);
    }


}
