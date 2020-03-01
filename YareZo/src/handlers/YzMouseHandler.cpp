#include "src/handlers/YzMouseHandler.h"
#include "Utilities/YzLogger.h"
#include "src/Application.h"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif


namespace Yarezo {


	MouseHandler::MouseHandler() {
		YZ_INFO("Mouse Input Handler Created");
	}

    MouseHandler::~MouseHandler() {
	}

	void MouseHandler::handle(std::shared_ptr<Camera> currentCamera) {
        //YZ_INFO("mouseX: " + STR(mouseX) + "\tmouseY: " + STR(mouseY));
        float yaw = currentCamera->getYaw();
        float pitch = currentCamera->getPitch();
        
        if (isFirstMouseInput) {
            prevMouseX = mouseX;
            prevMouseY = mouseY;
            isFirstMouseInput = false;
        }

        float deltaX = mouseX - prevMouseX;
        float deltaY = mouseY - prevMouseY;
        prevMouseX = mouseX;
        prevMouseY = mouseY;

        deltaX *= mouseSensitivity;
        deltaY *= mouseSensitivity;

        yaw += deltaX;
        pitch = glm::clamp(pitch + deltaY, -89.0f, 89.0f);

        currentCamera->setYaw(yaw);
        currentCamera->setPitch(pitch);
        
        YZ_INFO("yaw: " + STR(yaw) + "\tpitch: " + STR(pitch));

        glm::vec3 front;
        front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        front.y = -glm::sin(glm::radians(pitch));
        front.z = 0.5f; // glm::sin(glm::radians(yaw))* glm::cos(glm::radians(pitch));

        currentCamera->setLookAt(glm::normalize(front));
	}

}