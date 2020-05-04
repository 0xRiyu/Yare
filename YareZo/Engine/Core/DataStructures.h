#ifndef YAREZO_DATASTRUCTURES_H
#define YAREZO_DATASTRUCTURES_H

#include <glm/glm.hpp>

namespace Yarezo {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        bool operator==(const Vertex& other) const {
            return pos == other.pos && color == other.color && texCoord == other.texCoord;
        }
    };

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    struct UboDataDynamic {
        glm::mat4* model = nullptr;
    };

    struct UniformVS {
        glm::mat4 projection;
        glm::mat4 view;
    };
}

#endif
