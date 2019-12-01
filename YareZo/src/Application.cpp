//
// Created by Drew on 2019-08-15.
//

#include "src/Window.h"
#include "src/Application.h"
#include "Utilities/YzLogger.h"
#include "src/Vulkan.h"

namespace Yarezo {

    Application::Application() {
    }


    Application::~Application() {
    }

    void Application::Run() {

        Yarezo::YzLogger::Init();
        YZ_WARN("Logger Init Done");
        //Create a window
        WindowProperties props = {800, 600};

        auto window = Window::CreateNewWindow(props);
        GraphicsDevice_Vulkan vulkanDevice(window.get());

        while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(window->getNativeWindow()))) {
            window->OnUpdate();
            vulkanDevice.drawFrame();
        }

        vulkanDevice.waitIdle();

    }
}