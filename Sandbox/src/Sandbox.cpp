//
// Created by Drew on 2019-08-15.
// Sandbox is responsible for creating an Application,
// the public interfaces can be accessed and used here
//
#include <YareZo.h>

class Sandbox : public Yarezo::Application {
public:
    Sandbox() = default;

    ~Sandbox() override = default;
};

Yarezo::Application* Yarezo::createApplication() {
    return new Sandbox();
}