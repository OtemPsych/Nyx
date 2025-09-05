#pragma once

#include <chrono>
#include <source_location>
#include <string>
#include <string_view>
#include <utility>

#include "nyx/core/config.h"

namespace nyx::logging {
  /**
   * @brief A log entry stored in a logger which can then be flushed to one or more attached sinks.
   * 
   * Logs contain information pertaining to why the log was generated, as well as where in the program and when.
   */
  struct [[nodiscard]] NYX_API Log {
    /**
     * @brief The levels of severity for logs.
     * 
     * The levels of severity are ordered from least to most severe.
     */
    enum class Severity {
      Trace, //!< Can be used to query code parameters or analyse algorithm steps. Its purpose is primarily diagnostic.
      Debug, //!< Used to debug, troubleshoot, or test the application by providing diagnostic information, guaranteeing that the application is working as expected.
      Info, //!< Used to describe what occurred and the general state of the application. Anything logged at this level should be purely informative.
      Warn, //!< Used to indicate that something unexpected happened or may happen in the future. It is not necessarily an error, but it is worth noting.
      Error, //!< Used to indicate that something critical has failed, but the application can still continue running.
      Fatal //!< Used to indicate that something catastrophic has happened and the application cannot continue running.
    };

    std::string message;
    std::source_location location;
    std::chrono::system_clock::time_point timestamp;
    Severity severity{ Severity::Fatal };

    /**
     * @brief Formats the log according to the provided format string.
     * 
     * The format arguments are passed in the order: timestamp, severity, filename, line number, function name, and message.
     * 
     * @param fmt The formatting string for the log.
     * @return The formatted log.
     */
    [[nodiscard]] std::string to_string(std::string_view fmt) const;
  };

  [[nodiscard]] constexpr std::string_view to_string(Log::Severity severity) noexcept {
    switch (severity) {
      case Log::Severity::Trace: return "TRACE";
      case Log::Severity::Debug: return "DEBUG";
      case Log::Severity::Info: return "INFO";
      case Log::Severity::Warn: return "WARNING";
      case Log::Severity::Error: return "ERROR";
      case Log::Severity::Fatal: return "FATAL";
      default: std::unreachable();
    }
  }
}

// MIT License
//
// Copyright(c) 2025 Filippos Gleglakos
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
