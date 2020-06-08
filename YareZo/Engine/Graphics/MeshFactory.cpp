#include "Graphics/MeshFactory.h"
#include "Graphics/Mesh.h"
#include "Core/DataStructures.h"

namespace Yarezo::Graphics {

    Mesh* createMesh(PrimativeShape shape) {
        switch(shape) {
        case PrimativeShape::CUBE:
            return createCube(1.0f);
        // case PrimativeType::Quad:
        //     return createQuad(1.0f, 1.0f);
        // case PrimativeType::Sphere:
        //     return createSphere(1.0f);
        // case PrimativeType::Torus:
        //     return createTorus();
        // case PrimativeType::Rect:
        //     return createRect(1.0f, 1.0f, 2.0f);
        }
    }

    Mesh* createCube(float size) {

        /*
            e--------f
           / |     / |
          a--|----b  |
          |  g----|--h
          | /     | /
          c-------d
        */

        std::vector<Vertex> verts{20};

        verts[0].pos = glm::vec3(-size / 2, -size / 2, size / 2);
        verts[0].texCoord = glm::vec2(0.0, 1.0);
        verts[1].pos = glm::vec3(size / 2, -size / 2, size / 2);
        verts[1].texCoord = glm::vec2(1.0, 1.0);
        verts[2].pos = glm::vec3(-size / 2, size / 2, size / 2);
        verts[2].texCoord = glm::vec2(0.0, 0.0);
        verts[3].pos = glm::vec3(size / 2, size / 2, size / 2);
        verts[3].texCoord = glm::vec2(1.0, 0.0);
        verts[4].pos = glm::vec3(-size / 2, size / 2, size / 2);
        verts[4].texCoord = glm::vec2(0.0, 1.0);
        verts[5].pos = glm::vec3(size / 2, size / 2, size / 2);
        verts[5].texCoord = glm::vec2(1.0, 1.0);
        verts[6].pos = glm::vec3(-size / 2, size / 2, -size / 2);
        verts[6].texCoord = glm::vec2(0.0, 0.0);
        verts[7].pos = glm::vec3(size / 2, size / 2, -size / 2);
        verts[7].texCoord = glm::vec2(1.0, 0.0);
        verts[8].pos = glm::vec3(-size / 2, size / 2, -size / 2);
        verts[8].texCoord = glm::vec2(1.0, 0.0);
        verts[9].pos = glm::vec3(size / 2, size / 2, -size / 2);
        verts[9].texCoord = glm::vec2(0.0, 0.0);
        verts[10].pos = glm::vec3(-size / 2, -size / 2, -size / 2);
        verts[10].texCoord = glm::vec2(1.0, 1.0);
        verts[11].pos = glm::vec3(size / 2, -size / 2, -size / 2);
        verts[11].texCoord = glm::vec2(0.0, 1.0);
        verts[12].pos = glm::vec3(-size / 2, -size / 2, -size / 2);
        verts[12].texCoord = glm::vec2(0.0, 1.0);
        verts[13].pos = glm::vec3(size / 2, -size / 2, -size / 2);
        verts[13].texCoord = glm::vec2(1.0, 1.0);
        verts[14].pos = glm::vec3(-size / 2, -size / 2, size / 2);
        verts[14].texCoord = glm::vec2(0.0, 0.0);
        verts[15].pos = glm::vec3(size / 2, -size / 2, size / 2);
        verts[15].texCoord = glm::vec2(1.0, 0.0);
        verts[16].pos = glm::vec3(size / 2, -size / 2, size / 2);
        verts[16].texCoord = glm::vec2(0.0, 1.0);
        verts[17].pos = glm::vec3(size / 2, size / 2, size / 2);
        verts[17].texCoord = glm::vec2(0.0, 0.0);
        verts[18].pos = glm::vec3(-size / 2, -size / 2, size / 2);
        verts[18].texCoord = glm::vec2(1.0, 1.0);
        verts[19].pos = glm::vec3(-size / 2, size / 2, size / 2);
        verts[19].texCoord = glm::vec2(1.0, 0.0);

        for (auto vert : verts) {
            vert.color = glm::vec3(1.0f);
        }

        std::vector<uint32_t> inds = {0,   1,    2,
                                      2,   1,    3,
                                      4,   5,    6,
                                      6,   5,    7,
                                      8,   9,    10,
                                      10,  9,    11,
                                      12,  13,   14,
                                      14,  13,   15,
                                      16,  13,   17,
                                      17,  13,   7,
                                      12,  18,   6,
                                      6,   18,   19};

        return new Mesh(verts, inds);

    }

    // Mesh* createQuad(float width, float height) {

    // }

    // Mesh* createSphere(float diameter) {

    // }

    // Mesh* createTorus() {

    // }

    // Mesh* createRect(float width, float height, float depth) {

    // }
}
