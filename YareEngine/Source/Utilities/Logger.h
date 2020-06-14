#ifndef YARE_LOGGER_H
#define YARE_LOGGER_H
#include "Core/Core.h"

#include <memory>
#include <spdlog/spdlog.h>

namespace Yare {
    class Logger {
    public:
        static void init();
        static void changeFilePath(const std::string& path);
        static void update();
        inline static std::shared_ptr<spdlog::logger>& getEventLogger() { return m_EngineLogger; }

    private:
        static std::shared_ptr<spdlog::logger> m_EngineLogger;
        static std::string m_FileOutputPath;
        static std::string m_currentLogFileName;
    };
}

#define YZ_TRACE(...)       ::Yare::Logger::getEventLogger()->trace(__VA_ARGS__)
#define YZ_INFO(...)        ::Yare::Logger::getEventLogger()->info(__VA_ARGS__)
#define YZ_WARN(...)        ::Yare::Logger::getEventLogger()->warn(__VA_ARGS__)
#define YZ_ERROR(...)       ::Yare::Logger::getEventLogger()->error(__VA_ARGS__)
#define YZ_CRITICAL(...)    { \
                            ::Yare::Logger::getEventLogger()->critical(__VA_ARGS__); \
                            throw std::runtime_error(__VA_ARGS__); \
                            }

#define STR(x)              (std::to_string(x))

#endif //YARE_LOGGER_H
