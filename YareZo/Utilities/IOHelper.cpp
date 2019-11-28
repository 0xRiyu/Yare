//
// Created by Drew on 11/27/2019.
//
#include "IOHelper.h"
#include "YzLogger.h"

namespace Yarezo::Utilities {

    std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            YZ_ERROR("File '" + filename + "' was unable to open.");
            throw std::runtime_error("File '" + filename + "' was unable to open.");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

}