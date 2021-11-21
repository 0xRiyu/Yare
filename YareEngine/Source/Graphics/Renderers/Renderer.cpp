#include "Graphics/Renderers/Renderer.h"

namespace Yare::Graphics {

    void Renderer::resetCommandQueue() { m_CommandQueue.clear(); }

    void Renderer::submit(Entity* instance) {
        RenderCommand renderCommand;
        renderCommand.entity = instance;

        m_CommandQueue.push_back(renderCommand);
    }
}  // namespace Yare::Graphics
