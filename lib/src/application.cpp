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

#include "application.h"
#include "logger.h"
#include "raylib.h"

        int screenWidth = 800;
int screenHeight = 450;

namespace sneze {

int application::run() {
    LOG_DEBUG("Starting application");

    hook_raylib_log();

    LOG_DEBUG("Triggering On Start");
    on_start();

    LOG_DEBUG("Creating Window");

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    LOG_DEBUG("Window Created");

    while(!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    LOG_DEBUG("Closing Window");

    CloseWindow();

    LOG_DEBUG("Window Closed");

    LOG_DEBUG("Triggering On End");
    on_end();

    LOG_DEBUG("Stopping application");

    return 0;
}
application::application() {
#ifdef NDEBUG
    set_log_level(sneze::off);
#    if defined(_WIN32)
#        pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#    endif
#else
    set_log_level(sneze::info);
#endif
}

} // namespace sneze
