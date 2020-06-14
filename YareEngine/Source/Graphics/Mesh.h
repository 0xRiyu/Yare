#ifndef YARE_MESH_H
#define YARE_MESH_H

#include "Graphics/Vulkan/Buffer.h"
#include "Core/DataStructures.h"
#include <vector>

namespace Yare::Graphics {
    class Mesh {
    public:
        Mesh() {}
        Mesh(const std::string& meshFilePath);
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        virtual ~Mesh();

        Buffer* getIndexBuffer() const { return m_IndexBuffer; }
        Buffer* getVertexBuffer() const { return m_VertexBuffer; }

    protected:
        void createBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

        Buffer* m_VertexBuffer;
        Buffer* m_IndexBuffer;
        std::string m_FilePath;
    };
}

#endif //YARE_MESH_H
