#include "src/YzInput.h"
#include "Utilities/YzLogger.h"
#include "src/Application.h"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm.hpp>
#include <ext.hpp>
#include <gtx/string_cast.hpp>

namespace Yarezo {


    InputHandler::InputHandler() {

		for (int i = 0; i < MAX_KEYS; i++)
		{
			m_Keys[i] = false;
		}
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			m_Buttons[i] = false;
		}
		YZ_INFO("Input Handler Created");
    }

	InputHandler::~InputHandler() {
	}

	bool InputHandler::isKeyDown(int key) {
		if (key > MAX_KEYS) {
			YZ_WARN("Requested the status of a key " + std::to_string(key) +" that is outside the range of maximum keys.");
			return false;
		}
		return m_Keys[key];
	}

	void InputHandler::Handle(std::shared_ptr<Camera> currentCamera) {

		auto posVec = currentCamera->getPosition();
		auto upVec = currentCamera->getUpVector();
		auto lookAtVec = currentCamera->getLookAtVector();


		if (isKeyDown(GLFW_KEY_S)) {
			posVec += (glm::normalize(lookAtVec) * cameraSpeed);
		}

		if (isKeyDown(GLFW_KEY_W)) {
			posVec -= (glm::normalize(lookAtVec) * cameraSpeed);
		}

		if (isKeyDown(GLFW_KEY_A)) {
			posVec += (glm::normalize(glm::cross(lookAtVec, upVec)) * cameraSpeed);
		}

		if (isKeyDown(GLFW_KEY_D)) {
			posVec -= (glm::normalize(glm::cross(lookAtVec, upVec)) * cameraSpeed);
		}


		if (isKeyDown(GLFW_KEY_Q)) {
			posVec += upVec * cameraSpeed;
		}

		if (isKeyDown(GLFW_KEY_E)) {
			posVec -= upVec * cameraSpeed;
		}

	
		if (isKeyDown(GLFW_KEY_ESCAPE)) {
			YZ_INFO("Escape Key Pressed - Closing Window");
			Application::getAppInstance()->getWindow()->closeWindow();
		}

		currentCamera->setPosition(posVec);
	}

}