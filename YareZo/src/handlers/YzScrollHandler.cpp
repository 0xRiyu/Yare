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
        float newFov = glm::clamp(currFov - verticalScroll, 1.0f, 90.0f);
        YZ_INFO("currFov: " + STR(currFov) + "\tnewFov: " + STR(newFov) + "\tvScroll: " + STR(verticalScroll));
        currentCamera->setFov(newFov);
        
        // need to reset these because callback will not reset it
        verticalScroll = 0.0f;
        horizontalScroll = 0.0f;
    }

}