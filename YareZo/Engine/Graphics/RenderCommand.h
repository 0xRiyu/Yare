#ifndef YAREZO_RENDER_COMMAND_H
#define YAREZO_RENDER_COMMAND_H

#include "Graphics/Model.h"
#include "Platform/Vulkan/Vk_Image.h"

namespace Yarezo::Graphics {

    struct RenderCommand {
        Model* model;
        YzVkImage* texture;
        glm::mat4 transform;
    };
}

#endif // YAREZO_RENDER_COMMAND_H
