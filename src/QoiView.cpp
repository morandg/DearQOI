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

#include <sstream>

#include <imgui.h>

#define QOI_IMPLEMENTATION

#include "QoiView.h"
#include "Logger.h"

namespace DearQOI {

static constexpr float WINDOW_OFFSET = 50;

QoiView::QoiView(std::string imagePath) {
    void* imageData;
    ImGuiIO& io = ImGui::GetIO();
    std::stringstream ss;

    ss << imagePath << "##" << this;
    mWindowId = ss.str();

    // https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
    imageData = qoi_read(imagePath.c_str(), &mQoiDesc, 4);
    if (!imageData) {
        return;
    }

    glGenTextures(1, &mGlTextureId);
    glBindTexture(GL_TEXTURE_2D, mGlTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mQoiDesc.width, mQoiDesc.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    free(imageData);

    if (mQoiDesc.width > io.DisplaySize.x - WINDOW_OFFSET) {
        mWindowSize.x = io.DisplaySize.x - WINDOW_OFFSET;
    } else {
        mWindowSize.x = mQoiDesc.width + 50;
    }
    if (mQoiDesc.height > io.DisplaySize.y - WINDOW_OFFSET) {
        mWindowSize.y = io.DisplaySize.y - WINDOW_OFFSET;
    } else {
        mWindowSize.y = mQoiDesc.height + 70;
    }

    Logger::Main()->debug("Image size {}x{}", mQoiDesc.width, mQoiDesc.width);
    Logger::Main()->debug("Window size {}x{}", mWindowSize.x, mWindowSize.y);
}

QoiView::~QoiView() {
    if(mGlTextureId)
        glDeleteTextures(1, &mGlTextureId);
}

bool QoiView::update() {
    bool isOpened = true;

    ImGui::SetNextWindowPos({WINDOW_OFFSET, WINDOW_OFFSET}, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(mWindowSize, ImGuiCond_Appearing);

    ImGui::Begin(mWindowId.c_str(), &isOpened, ImGuiWindowFlags_HorizontalScrollbar);
    if (!mGlTextureId)
        ImGui::TextWrapped("Could not decode image!");
    else {
        ImGui::SliderFloat("Zoom", &mZoomLevel, 0.01, 5);
        ImGui::BeginChild("scrolling", {0,0}, false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Image((void *) (intptr_t) mGlTextureId,
                     ImVec2(mQoiDesc.width * mZoomLevel, mQoiDesc.height * mZoomLevel));
        ImGui::EndChild();
    }
    ImGui::End();

    return isOpened;
}

}