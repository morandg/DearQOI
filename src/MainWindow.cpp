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

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl2.h>

#include "Logger.h"
#include "MainWindow.h"

namespace DearQOI {

MainWindow::MainWindow() {
}

MainWindow::~MainWindow() {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if(mSdlWindow)
        SDL_DestroyWindow(mSdlWindow);
    if(mSdlGlContext)
        SDL_GL_DeleteContext(mSdlGlContext);

    SDL_Quit();
}

int MainWindow::init() {
    int status;

    status = initSdl();
    if (status)
        return status;
    status = initImgui();
    if (status)
        return status;

    Logger::Main()->info("Main window initialized!");

    return status;
}

bool MainWindow::update() {
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    drawWidgets();

    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(mSdlWindow);

    return mIsRunning;
}

void MainWindow::processEvent(const SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void MainWindow::drawWidgets() {
    drawMainMenu();

    if (mShowImguiDemo)
        ImGui::ShowDemoWindow(&mShowImguiDemo);
    if (mShowImguiMetrics)
        ImGui::ShowMetricsWindow(&mShowImguiMetrics);
    if (mShowImguiAbout)
        ImGui::ShowAboutWindow(&mShowImguiAbout);
    if (mShowAbout)
        drawAboutWindow();
    if (mShowLoadQoiDialog)
        drawLoadQoiDialog();

    for (auto qoiView = mQoiViews.begin(); qoiView != mQoiViews.end(); ++qoiView) {
        if (!(*qoiView)->update()) {
            qoiView = mQoiViews.erase(qoiView);
        }
    }
}

void MainWindow::drawMainMenu() {
    if( ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Load image"))
                mShowLoadQoiDialog = true;
            if (ImGui::MenuItem("Exit"))
                mIsRunning = false;
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options")) {
            if (ImGui::BeginMenu("Theme")) {
                if (ImGui::MenuItem("Dark"))
                    ImGui::StyleColorsDark();
                if (ImGui::MenuItem("Classic"))
                    ImGui::StyleColorsClassic();
                if (ImGui::MenuItem("Light"))
                    ImGui::StyleColorsLight();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Fonts")) {
                for (const auto &font: ImGui::GetIO().Fonts->Fonts) {
                    if (ImGui::MenuItem(font->GetDebugName())) {
                        ImGui::GetIO().FontDefault = font;
                    }
                }
                ImGui::EndMenu();
            }

            static ImVec4 bgColor;
            if (ImGui::ColorEdit3("Background color", (float *)&bgColor)) {
                glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
            }

            static bool wasFullScreen = false;
            bool isFullScreen = wasFullScreen;
            ImGui::Checkbox("Full screen", &isFullScreen);
            if (isFullScreen != wasFullScreen) {
                Uint32 sdlFlags = 0;

                wasFullScreen = isFullScreen;
                if(isFullScreen)
                    sdlFlags |= SDL_WINDOW_FULLSCREEN;
                SDL_SetWindowFullscreen(mSdlWindow, sdlFlags);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("DearIMGUI")) {
            if (ImGui::MenuItem("Demos"))
                mShowImguiDemo = true;
            if (ImGui::MenuItem("Metrics"))
                mShowImguiMetrics = true;
            if (ImGui::MenuItem("About"))
                mShowImguiAbout = true;
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About DearQOI"))
                mShowAbout = true;
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void MainWindow::drawAboutWindow() {
    ImGui::SetNextWindowPos( {50, 50}, ImGuiCond_Once);

    ImGui::Begin("About DearQOI", &mShowAbout,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::TextWrapped("DearIMGUI and QOI image format demo application for Bytes@Work");
    if (ImGui::Button("Close", {200, 0})) {
        Logger::Main()->info("Close about DearQOI clicked!");
        mShowAbout = false;
    }

    ImGui::End();
}

void MainWindow::drawLoadQoiDialog() {
    static constexpr int INPUT_BUFFER_SIZE = 512;
    static char inputText[INPUT_BUFFER_SIZE] = "../images/qoi_logo.qoi";

    ImGui::SetNextWindowPos( {50, 50}, ImGuiCond_Appearing);
    ImGui::Begin("Load QOI image", nullptr,
                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Enter a path to a QOI image");
    ImGui::InputText("##QoiPath", inputText, INPUT_BUFFER_SIZE);
    if (ImGui::Button("OK")) {
        auto qoiView = std::make_unique<QoiView>(inputText);
        mQoiViews.push_back(std::move(qoiView));
        mShowLoadQoiDialog = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
        mShowLoadQoiDialog = false;

    if (!mShowLoadQoiDialog)
        strncpy(inputText, "../images/qoi_logo.qoi", INPUT_BUFFER_SIZE);

    ImGui::End();
}

int MainWindow::initSdl() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
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
    if (!mSdlGlContext) {
        Logger::Main()->error("Error creating OpenGl context: {}", SDL_GetError());
        return -1;
    }

    if (SDL_GL_SetSwapInterval(0)) {
        Logger::Main()->error("Cannot set OpenGl swap interval: {}", SDL_GetError());
        return -1;
    }

    glClearColor(0, 0, 0, 1);

    return 0;
}

int MainWindow::initImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    if (!ImGui_ImplSDL2_InitForOpenGL(mSdlWindow, mSdlGlContext)) {
        Logger::Main()->error("Could not initialize DearIMGUI SDL2");
        return -1;
    }

    if (!ImGui_ImplOpenGL2_Init()) {
        Logger::Main()->error("Could not initialize DearIMGUI OpenGL");
        return -1;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../libs/imgui/misc/fonts/Cousine-Regular.ttf", 15);
    io.Fonts->AddFontFromFileTTF("../libs/imgui/misc/fonts/DroidSans.ttf", 15);
    io.Fonts->AddFontFromFileTTF("../libs/imgui/misc/fonts/Karla-Regular.ttf", 15);
    io.Fonts->AddFontFromFileTTF("../libs/imgui/misc/fonts/ProggyClean.ttf", 15);
    io.Fonts->AddFontFromFileTTF("../libs/imgui/misc/fonts/ProggyTiny.ttf", 15);
    io.Fonts->AddFontFromFileTTF("../libs/imgui/misc/fonts/Roboto-Medium.ttf", 15);

    return 0;
}

}
