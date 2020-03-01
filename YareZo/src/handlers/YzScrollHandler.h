#ifndef YZSCROLL_HPP
#define YZSCROLL_HPP

#include "src/YzCamera.h"
#include <GLFW/glfw3.h>
#include "src/handlers/Handler.h"

#include <memory>


namespace Yarezo {

    class ScrollHandler : public Handler {
    public:

        ScrollHandler();
        virtual ~ScrollHandler();

        virtual void handle(std::shared_ptr<Camera> currentCamera) override;

        float horizontalScroll = 0;
        float verticalScroll = 0;

    private:

    };


}
#endif