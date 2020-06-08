#include "Graphics/Vulkan/Vk_Framebuffer.h"
#include "Graphics/Vulkan/Vk_Devices.h"
#include "Utilities/Logger.h"

namespace Yarezo::Graphics {

    YzVkFramebuffer::YzVkFramebuffer(const FramebufferInfo& fbInfo) {

        VkFramebufferCreateInfo fbCreateInfo = {};
        fbCreateInfo.sType = fbInfo.type; // VK_STRUCTURE_TYPE_FB_CREATE_INFO;
        fbCreateInfo.renderPass = fbInfo.renderPass->getRenderPass();
        fbCreateInfo.attachmentCount = (uint32_t)fbInfo.attachments.size();
        fbCreateInfo.pAttachments = fbInfo.attachments.data();
        fbCreateInfo.width = fbInfo.width;
        fbCreateInfo.height = fbInfo.height;
        fbCreateInfo.layers = fbInfo.layers;

        auto res = vkCreateFramebuffer(YzVkDevice::instance()->getDevice(), &fbCreateInfo, nullptr, &m_Framebuffer);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to create a fb");
        }
    }

    YzVkFramebuffer::~YzVkFramebuffer() {
        if (m_Framebuffer) {
            vkDestroyFramebuffer(YzVkDevice::instance()->getDevice(), m_Framebuffer, nullptr);
        }
    }

}
