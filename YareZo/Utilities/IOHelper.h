//
// Created by Drew on 11/27/2019.
//

#ifndef YAREZO_IOHELPER_H
#define YAREZO_IOHELPER_H

#include <vector>
#include <fstream>
#include <string>

namespace Yarezo::Utilities {
    std::vector<std::string> readFile(const std::string& filename);
}

#endif //YAREZO_IOHELPER_H
