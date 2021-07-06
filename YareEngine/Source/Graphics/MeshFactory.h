#ifndef YARE_MESH_FACTORY_H
#define YARE_MESH_FACTORY_H

namespace Yare::Graphics {
    class Mesh;

    enum class PrimativeShape {
                               CUBE,
                               QUAD,
                               SPHERE,
                               TORUS,
                               RECT
    };

    // Create a mesh with its default parameters
    Mesh* createMesh(PrimativeShape shape);

    // Create a more custom mesh
    Mesh* createCube(float size);
    Mesh* createQuad(float width, float height);
    Mesh* createQuadPlane(size_t width, size_t height);
    // Mesh* createSphere(float diameter);
    // Mesh* createTorus();
    // Mesh* createRect(float width, float height, float depth);
}

#endif //YARE_MESH_FACTORY_H
