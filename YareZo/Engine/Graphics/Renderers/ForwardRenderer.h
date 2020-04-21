#ifndef YAREZO_FORWARD_RENDERER_H
#define YAREZO_FORWARD_RENDERER_H

#include "Graphics/Renderers/YzRenderer.h"

namespace Yarezo::Graphics {

    class ForwardRenderer : public YzRenderer {
    public:
        ForwardRenderer();
        ~ForwardRenderer() override;

        virtual void Init() override;

};

}

#endif // YAREZO_FORWARD_RENDERER_H
