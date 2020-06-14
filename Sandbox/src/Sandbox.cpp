#include <Yare.h>

class Sandbox : public Yare::Application {
public:
    Sandbox() = default;

    ~Sandbox() override = default;
};

Yare::Application* Yare::createApplication() {
    return new Sandbox();
}
