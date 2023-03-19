/****************************************************************************
MIT License

Copyright (c) 2023 Juan Medina

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

#pragma once

#include <string>

#include <fmt/format.h>

/**
 * @brief information about the current version
 *
 * This namespace contains information about the current version of the
 * library and includes the sneze ascii logo that will be printed when
 * the library is initialized.
 */
namespace sneze::version {

//! major version
static const int major = 0;

//! minor version
static const int minor = 0;

//! release version
static const int release = 0;

//! version string with format v{major}.{minor}.{release}
static const std::string string = fmt::format("v{}.{}.{}", major, minor, release);

//! sneze ascii logo template
static constexpr auto logo = R"(  ____
 / ___|   _ __     ___   ____   ___
 \___ \  | '_ \   / _ \ |_  /  / _ \
  ___) | | | | | |  __/  / /  |  __/
 |____/  |_| |_|  \___| /___|  \___|
                  version {}.{}.{})";

//! sneze ascii logo with version
static const auto logo_string = fmt::format(logo, major, minor, release);

} // namespace sneze::version
