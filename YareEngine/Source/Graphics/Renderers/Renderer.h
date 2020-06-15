#ifndef YARE_RENDERER_H
#define YARE_RENDERER_H

#include "Graphics/MeshInstance.h"
#include "Graphics/Vulkan/CommandBuffer.h"
#include "Graphics/Vulkan/RenderPass.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

namespace Yare::Graphics {

    struct RenderCommand {
        MeshInstance* meshInst;
    };

    typedef std::vector<RenderCommand> CommandQueue;

    class Renderer {
    public:
        virtual ~Renderer() = default;

        virtual void prepareScene() = 0;
        virtual void present(CommandBuffer* commandBuffer) = 0;
        virtual void onResize(RenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) = 0;

    protected:
        virtual void init(RenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) = 0;
        virtual void resetCommandQueue();
        virtual void submit(MeshInstance* instance);
        CommandQueue m_CommandQueue;
    };
}
#endif // YARE_RENDERER_H
