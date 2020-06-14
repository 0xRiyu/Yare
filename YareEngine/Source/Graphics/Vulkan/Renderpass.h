#ifndef YARE_RENDERPASS_H
#define YARE_RENDERPASS_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/CommandBuffer.h"

namespace Yare::Graphics {
    // Forward declaration
    class YzVkFramebuffer;

    struct RenderPassInfo {
        VkFormat imageFormat;
        VkExtent2D extent;
    };

    class YzVkRenderPass {
    public:
        YzVkRenderPass(const RenderPassInfo& info);
        ~YzVkRenderPass();

        void beginRenderPass(const YzVkCommandBuffer* commandBuffer, const YzVkFramebuffer* frameBuffer);
        void endRenderPass(const YzVkCommandBuffer* commandBuffer);

        const VkRenderPass& getRenderPass() const { return m_RenderPass; }
    private:
        void init();
        RenderPassInfo m_Info;

        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };
}
#endif //YARE_RENDERPASS_H
