#ifndef YARE_ENTRYPOINT_H
#define YARE_ENTRYPOINT_H

extern Yare::Application* Yare::createApplication();

int main() {
    auto app = Yare::createApplication();
    app->run();
    delete app;
}

#endif //YARE_ENTRYPOINT_H
