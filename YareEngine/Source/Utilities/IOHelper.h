#ifndef YARE_IOHELPER_H
#define YARE_IOHELPER_H

#include <fstream>
#include <string>
#include <vector>

#include "Core/DataStructures.h"

namespace Yare::Utilities {
    std::vector<std::string> readFile(const std::string& filename);

    void loadMesh(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
}  // namespace Yare::Utilities

#endif  // YARE_IOHELPER_H
