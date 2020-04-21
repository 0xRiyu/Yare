//
// Created by Drew on 2019-08-17.
//

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Utilities/YzLogger.h"

#include <ctime>
#include <iostream>
#include <string>
#include <ctime>

namespace Yarezo {

    std::shared_ptr<spdlog::logger> YzLogger::m_EngineLogger;
    std::string YzLogger::m_FileOutputPath;
    std::string YzLogger::m_currentLogFileName;

    void YzLogger::init() {
        // Get the Date/Time stamp to generate a new logfile
        tm current_time;
        time_t ltime  = std::time(nullptr);
        char buff[50];

        // Get local time then appent the logname to the end
        localtime_r(&ltime, &current_time);
        std::strftime(buff, sizeof(buff), "%Y-%m-%d_%H-%M-%S", &current_time);
        strcat(buff, "-YareZo_Engine_Log.txt");
        m_currentLogFileName = buff;

        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_st>("Logs/" + m_currentLogFileName));
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());

        m_EngineLogger = std::make_shared<spdlog::logger>("YareZo", begin(sinks), end(sinks));
    }

    void YzLogger::changeFilePath(const std::string& path) {
        m_FileOutputPath = path;
        update();
    }

    void YzLogger::update() {
        // Update the logger
    }
}
