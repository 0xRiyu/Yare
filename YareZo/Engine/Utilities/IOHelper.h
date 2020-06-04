#ifndef YAREZO_IOHELPER_H
#define YAREZO_IOHELPER_H

#include "Core/DataStructures.h"

#include <vector>
#include <fstream>
#include <string>

namespace Yarezo::Utilities {
    std::vector<std::string> readFile(const std::string& filename);

    void loadMesh(const std::string& meshFilePath,
                  std::vector<Vertex>& vertices,
                  std::vector<uint32_t>& indices);
}

#endif //YAREZO_IOHELPER_H
