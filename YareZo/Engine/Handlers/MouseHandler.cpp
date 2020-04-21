#include "Handlers/MouseHandler.h"
#include "Core/Yzh.h"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Yarezo {

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
	}

    void MouseHandler::handleMouseEvent() {
        auto rotation = p_Camera->getRotation();

        float deltaX = glm::clamp(currentMouseX - prevMouseX, -10.0f, 10.0f) * mouseSensitivity;
        float deltaY = glm::clamp(currentMouseY - prevMouseY, -10.0f, 10.0f) * mouseSensitivity;

        rotation.x += deltaX;
        rotation.y = glm::clamp(rotation.y + deltaY, -89.0f, 89.0f);

        p_Camera->setRotation(rotation);

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

}
