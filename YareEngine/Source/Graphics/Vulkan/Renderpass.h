#ifndef YARE_RENDERPASS_H
#define YARE_RENDERPASS_H

#include "Graphics/Vulkan/CommandBuffer.h"
#include "Graphics/Vulkan/Vk.h"

namespace Yare::Graphics {
    // Forward declaration
    class Framebuffer;

    struct RenderPassInfo {
        VkFormat   imageFormat;
        VkExtent2D extent;
    };

    class RenderPass {
       public:
        RenderPass(const RenderPassInfo& info);
        ~RenderPass();

        void beginRenderPass(const CommandBuffer* commandBuffer, const Framebuffer* frameBuffer);
        void endRenderPass(const CommandBuffer* commandBuffer);

        const VkRenderPass& getRenderPass() const { return m_RenderPass; }

       private:
        void           init();
        RenderPassInfo m_Info;

        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };
}  // namespace Yare::Graphics
#endif  // YARE_RENDERPASS_H
