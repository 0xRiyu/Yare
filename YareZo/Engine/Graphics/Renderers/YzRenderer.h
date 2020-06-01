#ifndef YAREZO_YZ_RENDERER_H
#define YAREZO_YZ_RENDERER_H

#include "Graphics/Model.h"
#include "Graphics/RenderCommand.h"

#include "Graphics/Vulkan/Vk_Renderer.h"
#include "Graphics/Vulkan/Vk_Framebuffer.h"
#include "Graphics/Vulkan/Vk_CommandBuffer.h"
#include "Graphics/Vulkan/Vk_RenderPass.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

namespace Yarezo::Graphics {

    typedef std::vector<RenderCommand> CommandQueue;

    class YzRenderer {
    public:
        virtual ~YzRenderer() = default;

        virtual void init(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) = 0;
        virtual void prepareScene() = 0;
        virtual void present(YzVkCommandBuffer* commandBuffer) = 0;
        virtual void onResize(YzVkRenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) = 0;
        virtual void resetCommandQueue() { m_CommandQueue.clear(); }

    protected:
        CommandQueue m_CommandQueue;
    };

}
#endif
