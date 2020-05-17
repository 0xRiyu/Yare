#ifndef YAREZO_RENDER_COMMAND_H
#define YAREZO_RENDER_COMMAND_H

#include "Graphics/Model.h"

namespace Yarezo::Graphics {

    struct RenderCommand {
        Model* model;
        glm::mat4 transform;
    };
}

#endif // YAREZO_RENDER_COMMAND_H
