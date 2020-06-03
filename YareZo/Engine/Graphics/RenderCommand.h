#ifndef YAREZO_RENDER_COMMAND_H
#define YAREZO_RENDER_COMMAND_H

#include "Graphics/MeshInstance.h"

namespace Yarezo::Graphics {

    struct RenderCommand {
        MeshInstance* meshInstance;
    };
}

#endif // YAREZO_RENDER_COMMAND_H
