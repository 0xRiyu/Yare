//
// Created by Drew on 2019-08-15.
//

#ifndef YAREZO_ENTRYPOINT
#define YAREZO_ENTRYPOINT

#ifdef YZ_PLATFORM_WINDOWS

extern Yarezo::Application* Yarezo::createApplication();

int main() {
    auto app = Yarezo::createApplication();
    app->run();
    delete app;
}

#endif

#endif //YAREZO_ENTRYPOINT
