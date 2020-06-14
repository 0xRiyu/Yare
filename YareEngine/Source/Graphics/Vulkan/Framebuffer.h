#ifndef YARE_FRAMEBUFFER_H
#define YARE_FRAMEBUFFER_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/Swapchain.h"
#include "Graphics/Vulkan/Renderpass.h"

namespace Yare::Graphics {

    struct FramebufferInfo {
        VkStructureType type;
        uint32_t width;
        uint32_t height;
        uint32_t layers = 1;
        std::vector<VkImageView> attachments;
        RenderPass* renderPass;
    };

    class Framebuffer {
    public:
        Framebuffer(const FramebufferInfo& fbInfo);
        ~Framebuffer();

        const VkFramebuffer& getFramebuffer() const { return m_Framebuffer; }

    private:
        VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;
    };
}
#endif //YARE_FRAMEBUFFER_H
