#pragma once

#ifdef _DEBUG
  constexpr bool NYX_DEBUG = true;

  #define NYX_ASSERT(condition, message)                                                                                                                                                    \
    do {                                                                                                                                                                                    \
      if (!(condition)) {                                                                                                                                                                   \
        const std::source_location location = std::source_location::current();                                                                                                              \
        std::cerr << std::format("[{}:{}:{}, {}] Assertion `{}` failed.\n{}\n\n", location.file_name(), location.line(), location.column(), location.function_name(), #condition, message); \
        std::abort();                                                                                                                                                                       \
      }                                                                                                                                                                                     \
    } while (false)
#else
  constexpr bool NYX_DEBUG = false;

  #define NYX_ASSERT(condition, message)
#endif

// Define macros to differentiate between static and dynamic linking (import/export macros)
#ifdef NYX_STATIC
  // Static builds don't need import/export macros
  #define NYX_API_EXPORT
  #define NYX_API_IMPORT
  #define NYX_API
#else
  #ifdef _WIN32
    // Windows compilers need specific keywords for importing and exporting
    #define NYX_API_EXPORT __declspec(dllexport)
    #define NYX_API_IMPORT __declspec(dllimport)

    // For Visual C++ compilers, we need to turn off the C4251 warning
    #ifdef _MSC_VER
      #pragma warning(disable: 4251)
    #endif

  #elif __GNUC__ >= 4 // Linux, FreeBSD
    #define NYX_API_EXPORT __attribute__ ((__visibility__ ("default")))
    #define NYX_API_IMPORT __attribute__ ((__visibility__ ("default")))
  #endif

  // Define the import/export macros
  #ifdef NYX_API_EXPORTING
    #define NYX_API NYX_API_EXPORT
  #else
    #define NYX_API NYX_API_IMPORT
  #endif
#endif

// MIT License
// 
// Copyright(c) 2025 Filippos Gleglakos
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
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
