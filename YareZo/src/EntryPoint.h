//
// Created by Drew on 2019-08-15.
//

#ifndef YAREZO_ENTRYPOINT
#define YAREZO_ENTRYPOINT

#ifdef YZ_PLATFORM_WINDOWS

#include <windows.h>

extern Yarezo::Application* Yarezo::createApplication();

int main() {
    HWND consoleWindow = GetConsoleWindow();
    SetWindowPos(consoleWindow, 0 , 500, 500, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    auto app = Yarezo::createApplication();
    app->run();
    delete app;
}

#endif

#endif //YAREZO_ENTRYPOINT
