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
        auto window = Window::CreateWindow(800, 600);
        GLFWwindow* windowInstance = static_cast<GLFWwindow*>(window->getNativeWindow());

        GraphicsDevice_Vulkan vulkanDevice;
        vulkanDevice.InitVulkan(windowInstance);
        window->OnUpdate();
    }
}