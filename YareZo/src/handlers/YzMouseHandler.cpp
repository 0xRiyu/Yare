#include "src/handlers/YzMouseHandler.h"
#include "Utilities/YzLogger.h"
#include "src/Application.h"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm.hpp>
#include <ext.hpp>
#include <gtx/string_cast.hpp>

namespace Yarezo {


	MouseHandler::MouseHandler() {
		YZ_INFO("Mouse Input Handler Created");
	}

    MouseHandler::~MouseHandler() {
	}

	void MouseHandler::handle(std::shared_ptr<Camera> currentCamera) {

        auto rotation = currentCamera->getRotation();

        if (isFirstMouseInput) {
            prevMouseX = mouseX;
            prevMouseY = mouseY;
            isFirstMouseInput = false;
        }

        float deltaX = glm::clamp(mouseX - prevMouseX, -10.0f, 10.0f) * mouseSensitivity;
        float deltaY = glm::clamp(mouseY - prevMouseY, -10.0f, 10.0f) * mouseSensitivity;

        rotation.x += deltaX;
        rotation.y = glm::clamp(rotation.y + deltaY, -89.0f, 89.0f);

        currentCamera->setRotation(rotation);        

        prevMouseX = mouseX;
        prevMouseY = mouseY;

	}

}