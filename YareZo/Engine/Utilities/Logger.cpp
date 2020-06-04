#include "Utilities/Logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <ctime>
#include <iostream>
#include <string>
#include <ctime>

namespace Yarezo {

    std::shared_ptr<spdlog::logger> Logger::m_EngineLogger;
    std::string Logger::m_FileOutputPath;
    std::string Logger::m_currentLogFileName;

    void Logger::init() {
        // Get the Date/Time stamp to generate a new logfile
        time_t timeinfo  = std::time(nullptr);
        char buff[50];
        tm* ltime;

        // Get local time then append the logname to the end
        // MSVC disable warning of non-secure localtime and strcat
        // localtime_s and strcat_s aren't available on unix gcc
#pragma warning( disable : 4996 )
        ltime = localtime(&timeinfo);
        std::strftime(buff, sizeof(buff), "%Y-%m-%d_%H-%M-%S", ltime);
        strcat(buff, "-YareZo_Engine_Log.txt");
#pragma warning ( default : 4996 )
        m_currentLogFileName = buff;

        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_st>("Logs/" + m_currentLogFileName));
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
        m_EngineLogger = std::make_shared<spdlog::logger>("YareZo", begin(sinks), end(sinks));
    }

    void Logger::changeFilePath(const std::string& path) {
        m_FileOutputPath = path;
        update();
    }

    void Logger::update() {
        // Update the logger
    }
}
