#include "Graphics/Renderers/YzRenderer.h"

namespace Yarezo::Graphics {

    void YzRenderer::resetCommandQueue() {
        m_CommandQueue.clear();
    }

    void YzRenderer::submitModel(Model* model, const glm::mat4& transform) {
        RenderCommand renderCommand;
        renderCommand.model = model;
        renderCommand.transform = transform;

        m_CommandQueue.push_back(renderCommand);
    }
}
