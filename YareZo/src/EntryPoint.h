//
// Created by Drew on 2019-08-15.
//

#ifndef YAREZO_ENTRYPOINT
#define YAREZO_ENTRYPOINT

#ifdef YZ_PLATFORM_WINDOWS
#include "../Utilities/YzLogger.h"

extern Yarezo::Application* Yarezo::CreateApplication();

int main() {
    Yarezo::YzLogger::Init();
    YZ_WARN("Logger Init Done");
    auto app = Yarezo::CreateApplication();
    YZ_WARN("Yarezo Application created");
    app->Run();
    delete app;
}

#endif

#endif //YAREZO_ENTRYPOINT
