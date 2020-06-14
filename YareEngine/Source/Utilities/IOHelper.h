#ifndef YARE_IOHELPER_H
#define YARE_IOHELPER_H

#include "Core/DataStructures.h"

#include <vector>
#include <fstream>
#include <string>

namespace Yare::Utilities {
    std::vector<std::string> readFile(const std::string& filename);

    void loadMesh(const std::string& filePath,
                  std::vector<Vertex>& vertices,
                  std::vector<uint32_t>& indices);
}

#endif //YARE_IOHELPER_H
