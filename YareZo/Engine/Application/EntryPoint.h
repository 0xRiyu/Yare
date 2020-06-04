#ifndef YAREZO_ENTRYPOINT_H
#define YAREZO_ENTRYPOINT_H

extern Yarezo::Application* Yarezo::createApplication();

int main() {
    auto app = Yarezo::createApplication();
    app->run();
    delete app;
}

#endif //YAREZO_ENTRYPOINT_H
