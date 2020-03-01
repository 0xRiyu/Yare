#include "src/handlers/YzScrollHandler.h"
#include "Utilities/YzLogger.h"
#include "src/Application.h"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif



namespace Yarezo {


    ScrollHandler::ScrollHandler() {
        YZ_INFO("Scroll Input Handler Created");
    }

    ScrollHandler::~ScrollHandler() {
    }

    void ScrollHandler::handle(std::shared_ptr<Camera> currentCamera) {
        float currFov = currentCamera->getFov();
        float newFov = glm::clamp(currFov - (float)verticalScroll, 1.0f, 90.0f);
        currentCamera->setFov(newFov);
    }

}