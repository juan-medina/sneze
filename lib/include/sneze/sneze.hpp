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

#include "app/application.hpp"
#include "app/config.hpp"
#include "app/settings.hpp"
#include "app/world.hpp"
#include "components/geometry.hpp"
#include "components/renderable.hpp"
#include "components/ui.hpp"
#include "device/keyboard.hpp"
#include "events/events.hpp"
#include "globals/globals.hpp"
#include "platform/error.hpp"
#include "platform/logger.hpp"
#include "platform/result.hpp"
#include "platform/version.hpp"
#include "render/font.hpp"
#include "render/render.hpp"
#include "render/resource.hpp"
#include "render/texture.hpp"
#include "systems/keys_system.hpp"
#include "systems/layout_system.hpp"
#include "systems/render_system.hpp"
#include "systems/sdl_events_system.hpp"
#include "systems/system.hpp"
