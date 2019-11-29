//
// Created by Drew on 2019-10-29.
//

#include "GlfwWindow.h"

namespace Yarezo {

    std::shared_ptr<Window> Window::CreateWindow(WindowProperties& properties) {
        return std::make_shared<GlfwWindow>(properties);
    }
    
    GlfwWindow::GlfwWindow(WindowProperties& properties) {
        m_Properties = properties;
        Init(properties);
    }

    GlfwWindow::~GlfwWindow() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void GlfwWindow::Init(WindowProperties& properties) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow(properties.width, properties.height, "Vulkan window", nullptr, nullptr);
    }
    
    void GlfwWindow::OnUpdate() {
        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();
        }
    }


}
