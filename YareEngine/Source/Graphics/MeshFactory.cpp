#include "Graphics/MeshFactory.h"

#include "Core/DataStructures.h"
#include "Graphics/Components/Mesh.h"

namespace Yare::Graphics {

    Mesh* createMesh(PrimativeShape shape) {
        switch (shape) {
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

        std::vector<Vertex> verticies;

        glm::f32 x = size / 2.0f;
        glm::f32 y = size / 2.0f;
        glm::f32 z = size / 2.0f;

        glm::vec3 a0 = glm::vec3(+x, +y, +z);
        glm::vec3 a1 = glm::vec3(-x, +y, +z);
        glm::vec3 a2 = glm::vec3(-x, -y, +z);
        glm::vec3 a3 = glm::vec3(+x, -y, +z);
        glm::vec3 a4 = glm::vec3(+x, +y, -z);
        glm::vec3 a5 = glm::vec3(-x, +y, -z);
        glm::vec3 a6 = glm::vec3(-x, -y, -z);
        glm::vec3 a7 = glm::vec3(+x, -y, -z);

        glm::vec3 verts[] = {a1, a2, a3, a3, a0, a1, a2, a6, a7, a7, a3, a2, a6, a5, a4, a4, a7, a6,
                             a5, a1, a0, a0, a4, a5, a0, a3, a7, a7, a4, a0, a5, a6, a2, a2, a1, a5};

        glm::vec2 texc[] = {
            glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1),
            glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1),
            glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1), glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(1, 0),
            glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1),
            glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(1, 1), glm::vec2(0, 1), glm::vec2(0, 0),
            glm::vec2(1, 1), glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1),
        };

        glm::vec3 norm[36];

        for (int i = 0; i < 36; i += 3) {
            glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(verts[i + 1]) - glm::vec3(verts[i]),
                                                         glm::vec3(verts[i + 2]) - glm::vec3(verts[i])));

            norm[i] = normal;
            norm[i + 1] = normal;
            norm[i + 2] = normal;
        }
        std::vector<uint32_t> inds;

        for (int i = 0; i < 36; i++) {
            Vertex new_vert;
            new_vert.pos = verts[i];
            new_vert.uv = texc[i];
            new_vert.normal = norm[i];
            verticies.push_back(new_vert);
            inds.push_back(i);
        }

        return new Mesh(verticies, inds);
    }

    Mesh* createQuad(float width, float height) {
        /*
        (-1, -1)   (1,-1)
            ----------
            |        |
            |        |
            |        |
            ----------
         (-1, 1)   (1, 1)
        */

        std::vector<Vertex>   vertices = {{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
                                        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
                                        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                                        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};
        std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};

        return new Mesh(vertices, indices);
    }

    Mesh* createQuadPlane(size_t width, size_t height) {
        float positionX = 0;
        float positionZ = 0;
        int   index = 0;

        size_t              vertex_count = (width - 1) * (height - 1) * 6;
        size_t              index_count = vertex_count;  // duplicating
        std::vector<Vertex> vertices;
        vertices.resize(vertex_count);
        std::vector<uint32_t> indices;
        indices.resize(index_count);

        size_t vertex_height = 0;  // Todo later with a heightmap
        size_t step_density = 1;

        for (size_t i = 0; i < height - 1; i++) {
            for (size_t j = 0; j < width - 1; j++) {
                // Triangle one
                // Upper left.
                positionX = (float)i;
                positionZ = (float)(j + 1);

                vertices[index].pos = glm::vec3(positionX, 0.0f, positionZ);
                vertices[index].uv = glm::vec2(1.0f, 1.0f);
                vertices[index].normal = glm::vec3(1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // Upper right.
                positionX = (float)(i + 1);
                positionZ = (float)(j + 1);

                vertices[index].pos = glm::vec3(positionX, 0.0f, positionZ);
                vertices[index].uv = glm::vec2(1.0f, 0.0f);
                vertices[index].normal = glm::vec3(1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // Bottom right.
                positionX = (float)(i + 1);
                positionZ = (float)j;

                vertices[index].pos = glm::vec3(positionX, 0.0f, positionZ);
                vertices[index].uv = glm::vec2(0.0f, 0.0f);
                vertices[index].normal = glm::vec3(1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // Triangle two
                // Bottom right.
                positionX = (float)(i + 1);
                positionZ = (float)j;

                vertices[index].pos = glm::vec3(positionX, 0.0f, positionZ);
                vertices[index].uv = glm::vec2(0.0f, 0.0f);
                vertices[index].normal = glm::vec3(1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // Bottom left.
                positionX = (float)i;
                positionZ = (float)j;

                vertices[index].pos = glm::vec3(positionX, 0.0f, positionZ);
                vertices[index].uv = glm::vec2(0.0f, 1.0f);
                vertices[index].normal = glm::vec3(1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;

                // Upper left.
                positionX = (float)i;
                positionZ = (float)(j + 1);

                vertices[index].pos = glm::vec3(positionX, 0.0f, positionZ);
                vertices[index].uv = glm::vec2(1.0f, 1.0f);
                vertices[index].normal = glm::vec3(1.0f, 1.0f, 1.0f);
                indices[index] = index;
                index++;
            }
        }
        return new Mesh(vertices, indices);
    }

    // Mesh* createSphere(float diameter) {

    // }

    // Mesh* createTorus() {

    // }

    // Mesh* createRect(float width, float height, float depth) {

    // }
}  // namespace Yare::Graphics
