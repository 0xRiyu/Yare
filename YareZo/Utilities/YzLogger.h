//
// Created by Drew on 2019-08-17.
//
#ifndef YAREZO_YZLOGGER
#define YAREZO_YZLOGGER

#include <memory>
#include <spdlog/spdlog.h>

#include "src/Core.h"

namespace Yarezo {
    class YzLogger {
    public:
        static void init();
        static void changeFilePath(const std::string& path);
        static void update();

        inline static std::shared_ptr<spdlog::logger>& getEventLogger() { return m_EngineLogger; }

    private:
        static std::shared_ptr<spdlog::logger> m_EngineLogger;
        static std::string m_FileOutputPath;
    };
}

#define YZ_TRACE(...)       ::Yarezo::YzLogger::getEventLogger()->trace(__VA_ARGS__)
#define YZ_INFO(...)        ::Yarezo::YzLogger::getEventLogger()->info(__VA_ARGS__)
#define YZ_WARN(...)        ::Yarezo::YzLogger::getEventLogger()->warn(__VA_ARGS__)
#define YZ_ERROR(...)       ::Yarezo::YzLogger::getEventLogger()->error(__VA_ARGS__)
#define YZ_CRITICAL(...)    ::Yarezo::YzLogger::getEventLogger()->critical(__VA_ARGS__)


#endif //YAREZO_YZLOGGER
