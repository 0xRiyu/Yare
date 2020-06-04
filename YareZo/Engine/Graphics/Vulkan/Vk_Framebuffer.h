#ifndef YAREZO_VK_FRAMEBUFFER_H
#define YAREZO_VK_FRAMEBUFFER_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/Vk_Swapchain.h"
#include "Graphics/Vulkan/Vk_Renderpass.h"

namespace Yarezo::Graphics {

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

        const VkFramebuffer& getFramebuffer() const { return m_Framebuffer; }

    private:
        VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;
    };
}
#endif //YAREZO_VK_FRAMEBUFFER_H
