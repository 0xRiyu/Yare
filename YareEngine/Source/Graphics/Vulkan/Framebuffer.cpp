#include "Graphics/Vulkan/Framebuffer.h"

#include "Graphics/Vulkan/Devices.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {

    Framebuffer::Framebuffer(const FramebufferInfo& fbInfo) {
        VkFramebufferCreateInfo fbCreateInfo = {};
        fbCreateInfo.sType = fbInfo.type;  // VK_STRUCTURE_TYPE_FB_CREATE_INFO;
        fbCreateInfo.renderPass = fbInfo.renderPass->getRenderPass();
        fbCreateInfo.attachmentCount = (uint32_t)fbInfo.attachments.size();
        fbCreateInfo.pAttachments = fbInfo.attachments.data();
        fbCreateInfo.width = fbInfo.width;
        fbCreateInfo.height = fbInfo.height;
        fbCreateInfo.layers = fbInfo.layers;

        auto res = vkCreateFramebuffer(Devices::instance()->getDevice(), &fbCreateInfo, nullptr, &m_Framebuffer);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to create a fb");
        }
    }

    Framebuffer::~Framebuffer() {
        if (m_Framebuffer) {
            vkDestroyFramebuffer(Devices::instance()->getDevice(), m_Framebuffer, nullptr);
        }
    }

}  // namespace Yare::Graphics
