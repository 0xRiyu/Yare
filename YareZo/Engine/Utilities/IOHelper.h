//
// Created by Drew on 11/27/2019.
//

#ifndef YAREZO_IOHELPER_H
#define YAREZO_IOHELPER_H

#include <vector>
#include <fstream>
#include <string>

#include "Core/DataStructures.h"

namespace Yarezo {

    namespace Utilities {

        std::vector<std::string> readFile(const std::string& filename);
        void loadMesh(const std::string& meshFilePath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
    }
}


#endif //YAREZO_IOHELPER_H
