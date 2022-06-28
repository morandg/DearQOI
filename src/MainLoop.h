/**
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *
 * Copyright (C) 2022 R4nd0m6uy <r4nd0m6uy@r4nd0m6uy.ch>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */

#pragma once

#include <SDL.h>

#include "MainWindow.h"
#include "Logger.h"

namespace DearQOI::MainLoop {

static constexpr Uint32 MAX_FPS = 60;
static constexpr Uint32 SHOW_FPS_INTERVAL = 1000;

inline int run(MainWindow &mainWindow) {
    int framesCount = 0;
    Uint32 lastTick = SDL_GetTicks();
    Uint32 lastFpsPrint = SDL_GetTicks();
    Uint32 frameDuration = 1000 / MAX_FPS;

    while (true) {
        SDL_Event event;

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                return 0;
            }
        }

        mainWindow.update();

        // Limit FPS
        lastTick = SDL_GetTicks();
        Uint32 thisFrameDuration = SDL_GetTicks() - lastTick;
        if (thisFrameDuration < frameDuration)
            SDL_Delay(frameDuration - thisFrameDuration);

        // Log FPS
        if (SDL_GetTicks() - lastFpsPrint >= SHOW_FPS_INTERVAL) {
            Logger::Main()->debug("{} FPS", framesCount);
            framesCount = 0;
            lastFpsPrint = SDL_GetTicks();
        } else
            ++framesCount;
    }
}

}