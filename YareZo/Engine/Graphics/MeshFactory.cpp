#include "Graphics/MeshFactory.h"
#include "Graphics/Mesh.h"
#include "Core/DataStructures.h"

namespace Yarezo::Graphics {

    Mesh* createMesh(PrimativeShape shape) {
        switch(shape) {
        case PrimativeShape::CUBE:
            return createCube(1.0f);
        case PrimativeShape::QUAD:
             return createQuad(1.0f, 1.0f);
        default:
            return nullptr;
        }
    }

    Mesh* createCube(float size) {

        /* You can't have a primative creation class without ascii art, like seriously
            e--------f
           / |     / |
          a--|----b  |
          |  g----|--h
          | /     | /
          c-------d
        */

        std::vector<Vertex> verts{20};
        verts[0].pos = glm::vec3(-size / 2, -size / 2, size / 2);
        verts[0].uv = glm::vec2(0.0, 1.0);
        verts[1].pos = glm::vec3(size / 2, -size / 2, size / 2);
        verts[1].uv = glm::vec2(1.0, 1.0);
        verts[2].pos = glm::vec3(-size / 2, size / 2, size / 2);
        verts[2].uv = glm::vec2(0.0, 0.0);
        verts[3].pos = glm::vec3(size / 2, size / 2, size / 2);
        verts[3].uv = glm::vec2(1.0, 0.0);
        verts[4].pos = glm::vec3(-size / 2, size / 2, size / 2);
        verts[4].uv = glm::vec2(0.0, 1.0);
        verts[5].pos = glm::vec3(size / 2, size / 2, size / 2);
        verts[5].uv = glm::vec2(1.0, 1.0);
        verts[6].pos = glm::vec3(-size / 2, size / 2, -size / 2);
        verts[6].uv = glm::vec2(0.0, 0.0);
        verts[7].pos = glm::vec3(size / 2, size / 2, -size / 2);
        verts[7].uv = glm::vec2(1.0, 0.0);
        verts[8].pos = glm::vec3(-size / 2, size / 2, -size / 2);
        verts[8].uv = glm::vec2(1.0, 0.0);
        verts[9].pos = glm::vec3(size / 2, size / 2, -size / 2);
        verts[9].uv = glm::vec2(0.0, 0.0);
        verts[10].pos = glm::vec3(-size / 2, -size / 2, -size / 2);
        verts[10].uv = glm::vec2(1.0, 1.0);
        verts[11].pos = glm::vec3(size / 2, -size / 2, -size / 2);
        verts[11].uv = glm::vec2(0.0, 1.0);
        verts[12].pos = glm::vec3(-size / 2, -size / 2, -size / 2);
        verts[12].uv = glm::vec2(0.0, 1.0);
        verts[13].pos = glm::vec3(size / 2, -size / 2, -size / 2);
        verts[13].uv = glm::vec2(1.0, 1.0);
        verts[14].pos = glm::vec3(-size / 2, -size / 2, size / 2);
        verts[14].uv = glm::vec2(0.0, 0.0);
        verts[15].pos = glm::vec3(size / 2, -size / 2, size / 2);
        verts[15].uv = glm::vec2(1.0, 0.0);
        verts[16].pos = glm::vec3(size / 2, -size / 2, size / 2);
        verts[16].uv = glm::vec2(0.0, 1.0);
        verts[17].pos = glm::vec3(size / 2, size / 2, size / 2);
        verts[17].uv = glm::vec2(0.0, 0.0);
        verts[18].pos = glm::vec3(-size / 2, -size / 2, size / 2);
        verts[18].uv = glm::vec2(1.0, 1.0);
        verts[19].pos = glm::vec3(-size / 2, size / 2, size / 2);
        verts[19].uv = glm::vec2(1.0, 0.0);

        for (auto vert : verts) {
            vert.normal = glm::vec3(1.0f);
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

    Mesh* createQuad(float width, float height) {
        std::vector<Vertex> vertices =
            {
             { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },
             { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },
             { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },
             { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } }
            };
        std::vector<uint32_t> indices = { 0,1,2, 2,3,0 };

        return new Mesh(vertices, indices);
    }

    // Mesh* createSphere(float diameter) {

    // }

    // Mesh* createTorus() {

    // }

    // Mesh* createRect(float width, float height, float depth) {

    // }
}
