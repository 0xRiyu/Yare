#ifndef YARE_MESH_H
#define YARE_MESH_H

#include "Component.h"
#include "Graphics/Vulkan/Buffer.h"
#include "Core/DataStructures.h"
#include <vector>

namespace Yare::Graphics {
    class Mesh : public Component {
    public:
        Mesh() {}
        Mesh(const std::string& meshFilePath);
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        virtual ~Mesh();

        void loadMeshFromFile(const std::string& meshFilePath);

        Buffer* getIndexBuffer() const { return m_IndexBuffer; }
        Buffer* getVertexBuffer() const { return m_VertexBuffer; }

    protected:
        void createBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

        Buffer* m_VertexBuffer = nullptr;
        Buffer* m_IndexBuffer = nullptr;
        std::string m_FilePath;
    };
}

#endif //YARE_MESH_H
