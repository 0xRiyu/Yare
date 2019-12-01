//
// Created by Drew on 2019-08-17.
//
#ifndef YAREZO_YZLOGGER
#define YAREZO_YZLOGGER

#include <memory>

#include "../src/Core.h"
#include <spdlog/spdlog.h>

namespace Yarezo {
    class YAREZO_API YzLogger {
    public:
        static void Init();
        static void ChangeFilePath(const std::string& path);
        static void Update();

        inline static std::shared_ptr<spdlog::logger>& GetEventLogger() { return m_Logger; }

    private:
        static std::shared_ptr<spdlog::logger> m_Logger;
        static std::string m_FileOutputPath;
    };
}

#define YZ_TRACE(...)       ::Yarezo::YzLogger::GetEventLogger()->trace(__VA_ARGS__)
#define YZ_INFO(...)        ::Yarezo::YzLogger::GetEventLogger()->info(__VA_ARGS__)
#define YZ_WARN(...)        ::Yarezo::YzLogger::GetEventLogger()->warn(__VA_ARGS__)
#define YZ_ERROR(...)       ::Yarezo::YzLogger::GetEventLogger()->error(__VA_ARGS__)

#endif //YAREZO_YZLOGGER
