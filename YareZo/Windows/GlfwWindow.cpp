//
// Created by Drew on 2019-10-29.
//

#include "GlfwWindow.h"

namespace Yarezo {

    std::shared_ptr<Window> Window::CreateWindow(unsigned short width, unsigned short height) {
        return std::make_shared<GlfwWindow>(width, height);
    }
    
    GlfwWindow::GlfwWindow(unsigned short width, unsigned short height) {
        Init(width, height);
    }

    GlfwWindow::~GlfwWindow() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void GlfwWindow::Init(unsigned short width, unsigned short height) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow(width, height, "Vulkan window", nullptr, nullptr);
    }
    
    void GlfwWindow::OnUpdate() {
        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();
        }
    }


}
