#include "Graphics/Renderers/Renderer.h"

namespace Yare::Graphics {

    void Renderer::resetCommandQueue() {
        m_CommandQueue.clear();
    }

    void Renderer::submit(MeshInstance* instance) {
        RenderCommand renderCommand;
        renderCommand.meshInst = instance;

        m_CommandQueue.push_back(renderCommand);
    }
}
