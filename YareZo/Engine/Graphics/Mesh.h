#ifndef YAREZO_MESH_H
#define YAREZO_MESH_H

#include "Platform/Vulkan/Vk_Buffer.h"
#include "Core/DataStructures.h"
#include <vector>

namespace Yarezo::Graphics {

    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        virtual ~Mesh();

        YzVkBuffer* getIndexBuffer() const { return m_IndexBuffer; }
        YzVkBuffer* getVertexBuffer() const { return m_VertexBuffer; }

    private:
        void createBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

        YzVkBuffer* m_VertexBuffer;
        YzVkBuffer* m_IndexBuffer;

    };
}

#endif //YAREZO_MESH_H
