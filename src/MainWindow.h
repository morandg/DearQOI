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

namespace DearQOI {

class MainWindow {
public:
    MainWindow();
    ~MainWindow();

    int init();
    bool update();
    void processEvent(const SDL_Event& event);

private:
    SDL_Window* mSdlWindow = nullptr;
    SDL_GLContext mSdlGlContext = nullptr;
    bool mIsRunning = true;
    bool mShowImguiDemo = false;
    bool mShowImguiMetrics = false;
    bool mShowImguiAbout = false;
    bool mShowAbout = false;

    void drawWidgets();
    void drawMainMenu();
    void drawAboutWindow();
    int initSdl();
    int initImgui();
};

}