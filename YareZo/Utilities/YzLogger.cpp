//
// Created by Drew on 2019-08-17.
//

#include "YzLogger.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace Yarezo {

    std::shared_ptr<spdlog::logger> YzLogger::m_Logger;
    std::string YzLogger::m_FileOutputPath;

    void YzLogger::Init() {
        m_Logger = spdlog::stdout_color_mt("YareZo");
    }
    void YzLogger::ChangeFilePath(const std::string& path) {
        m_FileOutputPath = path;
        Update();
    }

    void YzLogger::Update() {
        m_Logger = spdlog::stdout_color_mt("YareZo");
    }
}