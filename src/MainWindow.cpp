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

#include <SDL_opengl.h>

#include "Logger.h"
#include "MainWindow.h"

namespace DearQOI {

MainWindow::MainWindow() {
}

MainWindow::~MainWindow() {
    if(mSdlWindow)
        SDL_DestroyWindow(mSdlWindow);
    if(mSdlGlContext)
        SDL_GL_DeleteContext(mSdlGlContext);
    SDL_Quit();
}

int MainWindow::init() {
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        Logger::Main()->error("SDL initialization failed: {}", SDL_GetError());
        return -1;
    }

    if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)) {
        Logger::Main()->error("Cannot set OpenGl double buffering: {}", SDL_GetError());
        return -1;
    }

    if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)) {
        Logger::Main()->error("Cannot set OpenGl color deppth: {}", SDL_GetError());
        return -1;
    }

    mSdlWindow = SDL_CreateWindow("DearQOI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!mSdlWindow) {
        Logger::Main()->error("Error creating SDL window: {}", SDL_GetError());
        return -1;
    }

    mSdlGlContext = SDL_GL_CreateContext(mSdlWindow);
    if(!mSdlGlContext) {
        Logger::Main()->error("Error creating OpenGl context: {}", SDL_GetError());
        return -1;
    }

    if (SDL_GL_SetSwapInterval(0)) {
        Logger::Main()->error("Cannot set OpenGl swap interval: {}", SDL_GetError());
        return -1;
    }

    glClearColor(0, 0, 0, 1);

    Logger::Main()->info("Main window initialized!");

    return 0;
}

void MainWindow::update() {
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(mSdlWindow);
}

}