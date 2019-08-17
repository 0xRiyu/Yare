//
// Created by Drew on 2019-08-17.
//

#include "YzLogger.h"

namespace Yarezo {

    std::shared_ptr<spdlog::logger> YzLogger::m_Logger;
    std::string YzLogger::m_FileOutputPath;

    void YzLogger::Init() {
        m_FileOutputPath = "BasicLog.txt";
        m_Logger = spdlog::basic_logger_mt("basic_logger", m_FileOutputPath, true);
    }
    void YzLogger::ChangeFilePath(const std::string& path) {
        m_FileOutputPath = path;
        Update();
    }

    void YzLogger::Update() {
        m_Logger = spdlog::basic_logger_mt("basic_logger", m_FileOutputPath, false);
    }
}