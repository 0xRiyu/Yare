//
// Created by Drew on 2019-08-15.
//

#include "src/Window.h"
#include "src/Application.h"
#include "Utilities/YzLogger.h"
#include "src/Vulkan.h"
#include "src/YzCamera.h"

#include <imgui.h>

namespace Yarezo {

    Application::Application() {
    }


    Application::~Application() {
    }

    void Application::run() {

        Yarezo::YzLogger::init();
        YZ_WARN("Logger Initalized");
        //Create a window
        WindowProperties props = {800, 600};

        YZ_WARN(std::to_string(IMGUI_CHECKVERSION()));

        auto window = Window::createNewWindow(props);

        GraphicsDevice_Vulkan vulkanDevice(window);

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
            window->onUpdate();
        }

        vulkanDevice.waitIdle();

    }
}