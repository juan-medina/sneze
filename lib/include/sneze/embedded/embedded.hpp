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

//! embedded namespace
namespace sneze::embedded {

//! embedded sneze logo, in png format
static constexpr const char *const sneze_logo = "embedded://sneze_logo.png";
//! embedded mono spaced font, in fnt format
static constexpr const char *const mono_font = "embedded://sneze_mono.fnt";
//! embedded mono spaced font texture, in png format
static constexpr const char *const mono_font_texture = "embedded://mono_font.png";
//! embedded regular font, in fnt format
static constexpr const char *const regular_font = "embedded://regular_font.fnt";
//! embedded regular font texture, in png format
static constexpr const char *const regular_font_texture = "embedded://regular_font.png";

} // namespace sneze::embedded