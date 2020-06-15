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

    Application::Application() {
        if (s_AppInstance) {
            throw std::runtime_error("Attempted to create an Application after one has previously been created.");
        }
        s_AppInstance = this;
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
        m_Window = Graphics::Window::createNewWindow(props);

        Graphics::RenderManager renderManager{m_Window};

        auto previousFPSTime = glfwGetTime();
        auto previousFrameTime = glfwGetTime();
        int frameCount = 0;

        while (!m_Window->shouldClose()) {
            renderManager.renderScene();
            m_Window->onUpdate();

            // FPS
            {
                auto currentTime = glfwGetTime();
                auto deltaFPSTime = currentTime - previousFPSTime;
                m_Window->getCamera()->setCameraSpeed((float)(currentTime - previousFrameTime) * 5);
                if (deltaFPSTime >= 1.0) {
                    if (GlobalSettings::instance()->logFps) YZ_INFO("FPS: " + std::to_string(frameCount));
                    GlobalSettings::instance()->fps = frameCount;
                    previousFPSTime = currentTime;
                    frameCount = 0;
                }
                previousFrameTime = currentTime;
                frameCount++;
            }

        }
    }
}
