#include "Platform/Vulkan/Vk_Framebuffer.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Utilities/YzLogger.h"

namespace Yarezo {
    namespace Graphics {



        YzVkFramebuffer::YzVkFramebuffer(const FramebufferInfo& framebufferInfo) {

            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = framebufferInfo.type; // VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            //framebufferInfo.renderPass = m_RenderPass;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = &framebufferInfo.attachments;
            framebufferCreateInfo.width = framebufferInfo.width;
            framebufferCreateInfo.height = framebufferInfo.height;
            framebufferCreateInfo.layers = framebufferInfo.layers;

            if (vkCreateFramebuffer(YzVkDevice::instance()->getDevice(), &framebufferCreateInfo, nullptr, &m_Framebuffer) != VK_SUCCESS) {
                YZ_ERROR("Vulkan failed to create a framebuffer");
                throw std::runtime_error("Vulkan failed to create a framebuffer");
            }

        }

        YzVkFramebuffer::~YzVkFramebuffer() {
            if (m_Framebuffer) {
                vkDestroyFramebuffer(YzVkDevice::instance()->getDevice(), m_Framebuffer, nullptr);
            }

        }

    }
}