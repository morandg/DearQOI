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

namespace DearQOI {

QoiView::QoiView(std::string imagePath) {
    void* imageData;
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
}

QoiView::~QoiView() {
    if(mGlTextureId)
        glDeleteTextures(1, &mGlTextureId);
}

bool QoiView::update() {
    bool isOpened = true;

    ImGui::SetNextWindowPos({50, 50}, ImGuiCond_Appearing);

    if (mGlTextureId)
        ImGui::SetNextWindowSize(ImVec2(mQoiDesc.width + 30, mQoiDesc.height + 40), ImGuiCond_Appearing);
    else
        ImGui::SetNextWindowSize({200, 50}, ImGuiCond_Appearing);

    if (ImGui::Begin(mWindowId.c_str(), &isOpened, ImGuiWindowFlags_HorizontalScrollbar)) {
        if (!mGlTextureId)
            ImGui::TextWrapped("Could not decode image!");
        else
            ImGui::Image((void*)(intptr_t)mGlTextureId, ImVec2(mQoiDesc.width, mQoiDesc.height));
    }
    ImGui::End();

    return isOpened;
}

}