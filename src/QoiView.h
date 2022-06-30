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

#include <string>

#include <SDL_opengl.h>
#include <qoi.h>

namespace DearQOI {

class QoiView {
public:
    QoiView(std::string imagePath);
    ~QoiView();

    bool update();

private:
    std::string mWindowId;
    qoi_desc mQoiDesc;
    GLuint mGlTextureId = 0;
};

}