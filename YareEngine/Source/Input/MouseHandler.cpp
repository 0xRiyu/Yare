#include "Input/MouseHandler.h"
#include "Core/Yzh.h"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Yare {

    MouseHandler::MouseHandler(const std::shared_ptr<YzCamera> currentCamera)
    :p_Camera(currentCamera) {
        YZ_INFO("Mouse Input Handler Created");
    }

    MouseHandler::~MouseHandler() {
    }

    void MouseHandler::handle() {
        if (mouseEvent) {
            handleMouseEvent();
            mouseEvent = false;
        }
        if (scrollEvent) {
            handleScrollEvent();
            scrollEvent = false;
        }
        if (buttonEvent) {
            handleButtonEvent();
            buttonEvent = false;
        }
    }

    void MouseHandler::handleMouseEvent() {
        auto rotation = glm::degrees(p_Camera->getTransform().getVec3Rotation());

        float deltaX = glm::clamp(currentMouseX - prevMouseX, -10.0f, 10.0f) * mouseSensitivity;
        float deltaY = glm::clamp(currentMouseY - prevMouseY, -10.0f, 10.0f) * mouseSensitivity;

        rotation.x += deltaX;
        rotation.y = glm::clamp(rotation.y - deltaY, -89.0f, 89.0f);

        p_Camera->setRotation(glm::radians(rotation));

        prevMouseX = currentMouseX;
        prevMouseY = currentMouseY;
    }

    void MouseHandler::handleScrollEvent() {
        float currFov = p_Camera->getFov();
        float newFov = glm::clamp(currFov - m_VerticalScroll, 1.0f, 90.0f);

        p_Camera->setFov(newFov);

        // need to reset these because callback will not reset it
        m_VerticalScroll = 0.0f;
        m_HorizontalScroll = 0.0f;
    }

    void MouseHandler::handleButtonEvent() {
        if (mouseLeftButtonPressed || mouseRightButtonPressed)
            YZ_INFO("Someone clicked a button...");
        // Do nothing for now
    }

}
