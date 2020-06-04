#include "Graphics/Vulkan/Vk_Framebuffer.h"
#include "Graphics/Vulkan/Vk_Devices.h"
#include "Utilities/Logger.h"

namespace Yarezo::Graphics {

    YzVkFramebuffer::YzVkFramebuffer(const FramebufferInfo& framebufferInfo) {

        VkFramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.sType = framebufferInfo.type; // VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = framebufferInfo.renderPass->getRenderPass();
        framebufferCreateInfo.attachmentCount = (uint32_t)framebufferInfo.attachments.size();
        framebufferCreateInfo.pAttachments = framebufferInfo.attachments.data();
        framebufferCreateInfo.width = framebufferInfo.width;
        framebufferCreateInfo.height = framebufferInfo.height;
        framebufferCreateInfo.layers = framebufferInfo.layers;

        if (vkCreateFramebuffer(YzVkDevice::instance()->getDevice(), &framebufferCreateInfo, nullptr, &m_Framebuffer) != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to create a framebuffer");
        }
    }

    YzVkFramebuffer::~YzVkFramebuffer() {
        if (m_Framebuffer) {
            vkDestroyFramebuffer(YzVkDevice::instance()->getDevice(), m_Framebuffer, nullptr);
        }
    }

}
