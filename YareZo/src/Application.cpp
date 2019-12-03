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

        // Output some fps info to detemine if we nuke performace
        double previousTime = glfwGetTime();
        int frameCount = 0;

        while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(window->getNativeWindow()))) {

            double currentTime = glfwGetTime();
            frameCount++;

            // Output some fps info every 5s to detemine if we nuke performace
            if (currentTime - previousTime >= 5.0) {
                YZ_INFO("FPS: " + std::to_string(frameCount / 5));
                frameCount = 0;
                previousTime = currentTime;
            }

            vulkanDevice.drawFrame();
            window->OnUpdate();
        }

        vulkanDevice.waitIdle();

    }
}