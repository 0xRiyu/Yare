#ifndef YAREZO_VK_RENDERPASS_H
#define YAREZO_VK_RENDERPASS_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/Vk_CommandBuffer.h"

namespace Yarezo::Graphics {
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
#endif
