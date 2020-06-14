#include "TiledMesh.h"

namespace Yare::Graphics {
    TiledMesh::TiledMesh(uint32_t meshSize) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        vertices.reserve(meshSize * meshSize * 4);
        indices.reserve(meshSize * meshSize * 6);
        int index = 0;
        for (unsigned int x = 0; x < meshSize; x++) {
            for (unsigned int z = 0; z < meshSize; z++) {
                vertices.emplace_back(Vertex{ { x + 0.5f, 0.0f, z + 0.5f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } });
                vertices.emplace_back(Vertex{ { x - 0.5f, 0.0f, z + 0.5f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } });
                vertices.emplace_back(Vertex{ { x - 0.5f, 0.0f, z - 0.5f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } });
                vertices.emplace_back(Vertex{ { x + 0.5f, 0.0f, z - 0.5f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } });

                indices.emplace_back(0 + index);
                indices.emplace_back(1 + index);
                indices.emplace_back(2 + index);
                indices.emplace_back(2 + index);
                indices.emplace_back(3 + index);
                indices.emplace_back(0 + index);

                index += 4;
            }
        }
        createBuffers(vertices, indices);
    }
}
