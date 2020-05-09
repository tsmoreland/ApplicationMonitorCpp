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

#include <exception>
#include <memory>
#include <optional>
#include <string>

namespace Shared::Model {

    class CommandResult {
    public:
        [[nodiscard]] bool IsSuccess() const noexcept {
            return result;
        }
        [[nodiscard]] std::string const& GetMessage() const noexcept {
            return message;
        }
        [[nodiscard]] std::optional<std::exception> const& GetException() const noexcept {
            return exception;
        }

        explicit operator bool() const noexcept {
            return result;
        }

        static CommandResult Ok(std::string message = "") noexcept {
            return CommandResult(true, std::move(message), std::nullopt);
        }
        static CommandResult Fail(std::string message = "") noexcept {
            return CommandResult(true, std::move(message), std::nullopt);
        }
        static CommandResult Error(std::exception exception, std::string message = "") noexcept {
            return CommandResult(true, std::move(message), std::optional(exception));
        }

    private:
        bool result{false};
        std::string message;
        std::optional<std::exception> exception;

        CommandResult(bool const result, std::string message, std::optional<std::exception> exception = std::nullopt)
            : result{result}
            , message{std::move(message)}
            , exception{std::move(exception)} {
        }
            
    };

}
