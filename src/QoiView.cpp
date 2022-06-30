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

#include "QoiView.h"

namespace DearQOI {

QoiView::QoiView(std::string imagePath) {
    std::stringstream ss;

    ss << imagePath << "##" << this;
    mWindowId = ss.str();
}

bool QoiView::update() {
    ImGui::SetNextWindowPos({50, 50}, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize({200, 200});

    if (ImGui::Begin(mWindowId.c_str(), &mIsOpened)) {
        ImGui::TextWrapped("Here should the image be displayed");
    }
    ImGui::End();

    return mIsOpened;
}

}