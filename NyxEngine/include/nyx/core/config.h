#pragma once

#ifdef NDEBUG
  constexpr bool nyx_dbg = false;
#else
  constexpr bool nyx_dbg = true;
#endif // NDEBUG

// Differentiate between static and dynamic linking
#ifdef NYX_SHARED
  #ifdef _WIN32
    #define NYX_API_EXPORT __declspec(dllexport)
    #define NYX_API_IMPORT __declspec(dllimport)
  #elif __GNUC__ >= 4 // Linux, FreeBSD
    #define NYX_API_EXPORT __attribute__ ((__visibility__ ("default")))
    #define NYX_API_IMPORT __attribute__ ((__visibility__ ("default")))
  #endif // _WIN32

  #ifdef NYX_API_EXPORTING
    #define NYX_API NYX_API_EXPORT
  #else
    #define NYX_API NYX_API_IMPORT
  #endif // NYX_API_EXPORTING
#else
  #define NYX_API
#endif // NYX_SHARED

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