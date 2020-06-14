#include "Application/Application.h"
#include "Application/GlobalSettings.h"
#include "Utilities/Logger.h"
#include "Graphics/RenderManager.h"
#include "Core/Glfw.h"

// Define the header once here before anywhere else
// I dont have a better place to put this for now
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include <stb/stb_image.h>
#include <tinyobjloader/tiny_obj_loader.h>

#include <imgui/imgui.h>

#include <exception>

namespace Yare {

    Application* Application::s_AppInstance = nullptr;
    bool Application::logFPS = false;

    Application::Application() {
        if (s_AppInstance) {
            throw std::runtime_error("Attempted to create an Application after one has previously been created.");
        }
        s_AppInstance = this;
        GlobalSettings::instance();
        ImGui::CreateContext();
    }

    Application::~Application() {
        GlobalSettings::release();
        ImGui::DestroyContext();
    }

    void Application::run() {
        Yare::Logger::init();
        YZ_INFO("Logger Initialized");

        //Create a window
        Graphics::WindowProperties props = {1600, 1200};
        m_Window = Graphics::YzWindow::createNewWindow(props);

        Graphics::RenderManager renderManager{m_Window};

        double previousFPSTime = glfwGetTime();
        double previousFrameTime = glfwGetTime();
        int frameCount = 0;

        while (!m_Window->shouldClose()) {
            double currentTime = glfwGetTime();
            frameCount++;
            double deltaFrameTime = currentTime - previousFrameTime;
            double deltaFPSTime = currentTime - previousFPSTime;
            m_Window->getCamera()->setCameraSpeed((float)deltaFrameTime * 5);
            // Output some fps info every 5s to determine if we nuke performace
            if (deltaFPSTime >= 1.0) {
                if (logFPS) YZ_INFO("FPS: " + std::to_string(frameCount));
                GlobalSettings::instance()->fps = frameCount;
                frameCount = 0;
                previousFPSTime = currentTime;
            }

            previousFrameTime = currentTime;

            renderManager.renderScene();

            m_Window->onUpdate();
        }

        renderManager.waitDeviceIdle();
    }
}
