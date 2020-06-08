#include "Graphics/Renderers/YzRenderer.h"

namespace Yarezo::Graphics {

    void YzRenderer::resetCommandQueue() {
        m_CommandQueue.clear();
    }

    void YzRenderer::submit(MeshInstance* instance) {
        RenderCommand renderCommand;
        renderCommand.meshInst = instance;

        m_CommandQueue.push_back(renderCommand);
    }
}
