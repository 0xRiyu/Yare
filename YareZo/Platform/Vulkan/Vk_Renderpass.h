#ifndef YAREZO_VK_RENDERPASS_H
#define YAREZO_VK_RENDERPASS_H

#include "Platform/Vulkan/Vk.h"


namespace Yarezo {
    namespace Graphics {

        struct RenderPassInfo {
            VkFormat imageFormat;
        };

        class YzVkRenderPass {
        public:
            YzVkRenderPass();
            ~YzVkRenderPass();

            void init(const RenderPassInfo& renderPassInfo);
            void cleanUp();

            inline const VkRenderPass& getRenderPass() const { return m_RenderPass; }

        private:
            VkRenderPass m_RenderPass;
        };

    }
}

#endif