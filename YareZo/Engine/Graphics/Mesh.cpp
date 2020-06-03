#include "Graphics/Mesh.h"
#include "Utilities/IOHelper.h"

#include "Core/yzh.h"

namespace Yarezo::Graphics {

    Mesh::Mesh(const std::string& meshFilePath)
        :m_FilePath(meshFilePath) {
        if (!meshFilePath.empty()) {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            Utilities::loadMesh(meshFilePath, vertices, indices);
            createBuffers(vertices, indices);
        }
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
        createBuffers(vertices, indices);
    }

    Mesh::~Mesh() {
        delete m_VertexBuffer;
        delete m_IndexBuffer;
    }

    void Mesh::createBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
        // Vertex Buffers
        VkDeviceSize bufferSize = sizeof(Vertex) * vertices.size();
        VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        VkMemoryPropertyFlags propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        m_VertexBuffer = new YzVkBuffer(usageFlags, propertyFlags, (size_t)bufferSize, vertices.data());

        // Index Buffers
        bufferSize = sizeof(indices[0]) * indices.size();
        usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

        m_IndexBuffer = new YzVkBuffer(usageFlags, propertyFlags, (size_t)bufferSize, indices.data());

    }
}
