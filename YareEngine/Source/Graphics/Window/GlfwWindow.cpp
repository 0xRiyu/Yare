#include "Graphics/Window/GlfwWindow.h"
#include "Graphics/Camera/Camera.h"
#include "Input/KeyHandler.h"
#include "Input/MouseHandler.h"
#include "Core/Yzh.h"

#include <imgui/imgui.h>
#include <stb/stb_image.h>

namespace Yare::Graphics {

    std::shared_ptr<YzWindow> YzWindow::createNewWindow(WindowProperties& properties) {
        return std::make_shared<GlfwWindow>(properties);
    }

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        if (width == 0 || height == 0) {
            YZ_INFO("Application was minimized.");
            while (width == 0 || height == 0) {
                glfwGetFramebufferSize(window, &width, &height);
                glfwWaitEvents();
            }
            YZ_INFO("Application is no longer minimized.");
        }
        YZ_INFO("The application window has been re-sized, the new dimensions [W,H]  are: "
                + std::to_string(width) + ", " + std::to_string(height));
        auto glfwWindow  = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        glfwWindow->setWindowProperties(WindowProperties{(uint32_t)width, (uint32_t)height});
    }

    static void GLFWkeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto keyHandler = std::dynamic_pointer_cast<KeyHandler>
            (Application::getAppInstance()->getWindow()->getKeyHandler());

        keyHandler->m_KeyState[key] = action;
    }

    static void GLFWmouseCallback(GLFWwindow* window, double mouseX, double mouseY) {
        auto mouseHandler = std::dynamic_pointer_cast<MouseHandler>
            (Application::getAppInstance()->getWindow()->getMouseHandler());

        mouseHandler->currentMouseX = (float)mouseX;
        mouseHandler->currentMouseY = (float)mouseY;
        mouseHandler->mouseEvent = true;
    }

    static void GLFWmouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto mouseHandler = std::dynamic_pointer_cast<MouseHandler>
            (Application::getAppInstance()->getWindow()->getMouseHandler());

        mouseHandler->mouseLeftButtonPressed = (button = GLFW_MOUSE_BUTTON_LEFT && action) ? true : false;
        mouseHandler->mouseRightButtonPressed = (button = GLFW_MOUSE_BUTTON_RIGHT && action) ? true : false;
        mouseHandler->buttonEvent = true;

        // GUI related
        {
            ImGuiIO& io = ImGui::GetIO();
            if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) ||
                glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
                return;
            }

            io.MousePos = ImVec2(mouseHandler->currentMouseX, mouseHandler->currentMouseY);
            io.MouseDown[0] = mouseHandler->mouseRightButtonPressed;
            io.MouseDown[1] = mouseHandler->mouseLeftButtonPressed;
            io.MouseClicked[0] = mouseHandler->mouseRightButtonPressed;
            io.MouseClicked[1] = mouseHandler->mouseLeftButtonPressed;
        }
    }

    static void GLFWscrollCallback(GLFWwindow* window, double horizontalScroll, double verticalScroll) {
        auto mouseHandler = std::dynamic_pointer_cast<MouseHandler>
            (Application::getAppInstance()->getWindow()->getMouseHandler());

        mouseHandler->setHorizontalScroll((float)horizontalScroll);
        mouseHandler->setVerticalScroll((float)verticalScroll);
        mouseHandler->scrollEvent = true;
    }

    GlfwWindow::GlfwWindow(WindowProperties& properties) {
        m_Properties = properties;
        // TODO: A scene should be the camera's over, we just dont have scenes yet
        m_Camera = std::make_shared<Graphics::Camera>(m_Properties.width, m_Properties.height);
        m_KeyHandler = std::make_shared<KeyHandler>(m_Camera);
        m_MouseHandler = std::make_shared<MouseHandler>(m_Camera);
        init();
    }

    GlfwWindow::~GlfwWindow() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();

    }

    void GlfwWindow::init() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow(m_Properties.width, m_Properties.height, "YareZo!", nullptr, nullptr);
        glfwSetWindowUserPointer(m_Window, this);
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // capture cursor
        setIcon("..\\YareZo\\Resources\\Textures\\engineLogo.png");

        // Callbacks
        glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallback);
        glfwSetKeyCallback(m_Window, GLFWkeyCallback);
        glfwSetCursorPosCallback(m_Window, GLFWmouseCallback);
        glfwSetMouseButtonCallback(m_Window, GLFWmouseButtonCallback);
        glfwSetScrollCallback(m_Window, GLFWscrollCallback);
    }

    void GlfwWindow::onUpdate() {
        glfwPollEvents();
        if (windowIsFocused) {
            m_KeyHandler->handle();
            m_MouseHandler->handle();
        }
        // Dear ImGui documentation very clearly states NOT to do this,
        // but I couldn't figure out how to do it in the way I wanted and this seems to work
        else if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow)) {
            windowIsFocused = true;
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        windowResized = false;
    }

    bool GlfwWindow::shouldClose() {
        return glfwWindowShouldClose(m_Window);
    }

    void GlfwWindow::close() {
        glfwSetWindowShouldClose(m_Window, 1);
    }


    void GlfwWindow::releaseInputHandling() {
        if (windowIsFocused) {
            windowIsFocused = false;
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPos(m_Window, m_Properties.width / 2, m_Properties.height / 2);
            auto& io = ImGui::GetIO();
            io.WantCaptureMouse = true;
            ImGui::SetNextWindowFocus();
        }
    }

    void GlfwWindow::setIcon(const std::string& filePath) {
        GLFWimage image;
        int imgWidth, imgHeight, imgChannels;
        stbi_uc* pixels = stbi_load(filePath.data(), &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);
        image.height = imgHeight;
        image.width = imgWidth;
        image.pixels = pixels;
        glfwSetWindowIcon(m_Window, 1, &image);
    }
}
