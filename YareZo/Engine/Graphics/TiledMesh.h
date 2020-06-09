#ifndef YAREZO_TILED_MESH_H
#define YAREZO_TILED_MESH_H

#include "Graphics/Mesh.h"

namespace Yarezo::Graphics {


    class TiledMesh : public Mesh {
    public:
        TiledMesh(uint32_t meshSize = 1);

    };

}
#endif //YAREZO_TILED_MESH_H
