//
// Created by Drew on 2019-08-15.
//

#include "Window.h"
#include "Application.h"

namespace Yarezo {

    Application::Application() {
    }


    Application::~Application() {
    }

    void Application::Run() {
        auto window = Window::CreateWindow(800, 600);
        window->OnUpdate();
    }
}