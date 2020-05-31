//
// Created by Drew on 2019-08-15.
//

#include "Application/Application.h"
#include "Utilities/YzLogger.h"
#include "Graphics/Renderers/ForwardRenderer.h"
#include "Core/Glfw.h"

// Define the header once here before anywhere else
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include <stb/stb_image.h>
#include <tinyobjloader/tiny_obj_loader.h>

#include <exception>

namespace Yarezo {

    Application* Application::s_AppInstance = nullptr;
    bool Application::logFPS = false;

    Application::Application() {
        if (s_AppInstance) {
            throw std::runtime_error("Application already exists, Don't be doing that fam");
        }
        s_AppInstance = this;
    }

    Application::~Application() {
    }


    void Application::run() {
        Yarezo::YzLogger::init();
        YZ_INFO("Logger Initialized");

        //Create a window
        WindowProperties props = {1600, 1200};
        m_Window = YzWindow::createNewWindow(props);

        // Create the vulkan pipeline
        // GraphicsDevice_Vulkan vulkanDevice;
        Graphics::ForwardRenderer forwardRenderer;


        double previousFPSTime = glfwGetTime();
        double previousFrameTime = glfwGetTime();
        int frameCount = 0;

        while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window->getNativeWindow()))) {
            double currentTime = glfwGetTime();
            frameCount++;
            double deltaFrameTime = currentTime - previousFrameTime;
            double deltaFPSTime = currentTime - previousFPSTime;
            m_Window->getCamera()->setCameraSpeed((float)deltaFrameTime * 5);
            // Output some fps info every 5s to determine if we nuke performace
            if (deltaFPSTime >= 1.0) {
                if (logFPS) YZ_INFO("FPS: " + std::to_string(frameCount));
                frameCount = 0;
                previousFPSTime = currentTime;
            }
            previousFrameTime = currentTime;

            forwardRenderer.renderScene();
            m_Window->onUpdate();
        }
        forwardRenderer.waitIdle();
    }
}
