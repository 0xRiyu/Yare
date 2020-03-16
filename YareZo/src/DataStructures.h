#ifndef YAREZO_DATASTRUCTURES_H
#define YAREZO_DATASTRUCTURES_H

#include <glm.hpp>

namespace Yarezo {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        bool operator==(const Vertex& other) const {
            return pos == other.pos && color == other.color && texCoord == other.texCoord;
        }
    };
}

#endif