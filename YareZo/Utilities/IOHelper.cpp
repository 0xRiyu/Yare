//
// Created by Drew on 11/27/2019.
//
#include "IOHelper.h"
#include "YzLogger.h"

namespace Yarezo::Utilities {

    std::vector<std::string> readFile(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            YZ_ERROR("File '" + filename + "' was unable to open.");
            throw std::runtime_error("File '" + filename + "' was unable to open.");
        }

        // captures lines not separated by whitespace
        std::vector<std::string> myLines;
        std::copy(std::istream_iterator<std::string>(file),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(myLines));

        return myLines;
    }

}