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
#include <TlHelp32.h>
#include "process.h"

namespace shared::model
{
    class process_impl final : public process
    {
    public:
        static unique_process start(std::string_view const& filename, std::string_view const& arguments);
        static std::vector<unique_process> get_processes_by_name(std::string_view const& process_name);

        [[nodiscard]] unsigned long get_id() const noexcept final;
        [[nodiscard]] bool is_running() const noexcept final;
        [[nodiscard]] std::optional<unsigned long> exit_code() const noexcept final;
        void wait_for_exit() const noexcept final; 
        [[nodiscard]] std::optional<std::filesystem::path> get_path_to_running_process(std::string_view const& process_name) const noexcept final;

        process_impl() = default;
        explicit process_impl(unsigned long const process_id);
        process_impl(const process_impl&) = delete;
        process_impl& operator=(const process_impl&) = delete;
        process_impl(process_impl&& other) noexcept;
        process_impl& operator=(process_impl&& other) noexcept;
        ~process_impl();

        [[nodiscard]] bool equals(process_impl const& other) const noexcept;
    private:
        bool m_process_launched{};
        unsigned long m_process_id{};
        unsigned long m_process_thread_id{};
        shared::infrastructure::null_handle m_process_handle{};
        shared::infrastructure::null_handle m_process_thread_handle{};

        explicit process_impl(PROCESS_INFORMATION const& process_information);
        static bool create_process_adapter(std::string_view const& filename, std::string_view const& arguments, STARTUPINFOA * const startup_info, PROCESS_INFORMATION * const process_info);
        static std::tuple<bool, unsigned long> get_running_details(HANDLE process_handle);

        static std::optional<PROCESSENTRY32> get_process_by_name(std::string_view const& process_name);
        static std::vector<PROCESSENTRY32> get_process_entries();
        static std::optional<MODULEENTRY32> get_module_by_id_and_name(unsigned long const process_id, std::string_view const& process_name);
        static std::vector<MODULEENTRY32> get_module_entries(unsigned long const process_id);
    };

    bool operator==(process_impl const& left_hand_side, process_impl const& right_hand_side);

}

