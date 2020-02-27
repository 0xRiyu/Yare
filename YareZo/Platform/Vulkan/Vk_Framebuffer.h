#ifndef YAREZO_VK_FRAMEBUFFER_H
#define YAREZO_VK_FRAMEBUFFER_H

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_Swapchain.h"
#include "Platform/Vulkan/Vk_Renderpass.h"

namespace Yarezo {
    namespace Graphics {

        struct FramebufferInfo {
            VkStructureType type;
            uint32_t width;
            uint32_t height;
            uint32_t layers = 1;
            std::vector<VkImageView> attachments;
            YzVkRenderPass* renderPass;
        };

        class YzVkFramebuffer {


        public:
            YzVkFramebuffer(const FramebufferInfo& framebufferInfo);
            ~YzVkFramebuffer();

            void cleanUp();
            const VkFramebuffer& getFramebuffer() const { return m_Framebuffer; }

        private:
            VkFramebuffer m_Framebuffer = nullptr;

        };
    }
}



#endif