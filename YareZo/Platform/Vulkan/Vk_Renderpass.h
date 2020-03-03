#ifndef YAREZO_VK_RENDERPASS_H
#define YAREZO_VK_RENDERPASS_H

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_CommandBuffer.h"
#include "Platform/Vulkan/Vk_Swapchain.h"

namespace Yarezo {
    namespace Graphics {

        // Forward declaration
        class YzVkFramebuffer;

        struct RenderPassInfo {
            VkFormat imageFormat;
        };

        class YzVkRenderPass {
        public:
            YzVkRenderPass();
            ~YzVkRenderPass();

            void init(const RenderPassInfo& renderPassInfo);
            void cleanUp();
            void beginRenderPass(YzVkCommandBuffer* const commandBuffer, YzVkFramebuffer* const frameBuffer, YzVkSwapchain* const swapchain);
            void endRenderPass(YzVkCommandBuffer* const commandBuffer);

            const VkRenderPass& getRenderPass() const { return m_RenderPass; }

        private:
            VkRenderPass m_RenderPass;
        };

    }
}

#endif