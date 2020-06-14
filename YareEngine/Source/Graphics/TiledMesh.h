#ifndef YARE_TILED_MESH_H
#define YARE_TILED_MESH_H

#include "Graphics/Mesh.h"

namespace Yare::Graphics {


    class TiledMesh : public Mesh {
    public:
        TiledMesh(uint32_t meshSize = 1);

    };

}
#endif //YARE_TILED_MESH_H
