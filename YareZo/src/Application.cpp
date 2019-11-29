//
// Created by Drew on 2019-08-15.
//

#include "Window.h"
#include "Vulkan.h"
#include "Application.h"

namespace Yarezo {

    Application::Application() {
    }


    Application::~Application() {
    }

    void Application::Run() {
        //Create a window
        WindowProperties props = {800, 600};

        auto window = Window::CreateWindow(props);

        GraphicsDevice_Vulkan vulkanDevice(window.get());

        while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(window->getNativeWindow()))) {
            window->OnUpdate();
            vulkanDevice.drawFrame();
        }

        vulkanDevice.waitIdle();

    }
}