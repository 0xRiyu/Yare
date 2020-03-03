#include "Platform/Vulkan/Vk_Renderpass.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Platform/Vulkan/Vk_Framebuffer.h"
#include "Utilities/YzLogger.h"


namespace Yarezo {

    namespace Graphics {
        YzVkRenderPass::YzVkRenderPass() {
        }
        YzVkRenderPass::~YzVkRenderPass() {
        }


        void YzVkRenderPass::init(const RenderPassInfo& renderPassInfo) {

            VkAttachmentDescription colorAttachment = {};
            colorAttachment.format = renderPassInfo.imageFormat;
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference colorAttachmentRef = {};
            colorAttachmentRef.attachment = 0;
            colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass = {};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &colorAttachmentRef;

            VkSubpassDependency dependency = {};
            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency.dstSubpass = 0;
            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.srcAccessMask = 0;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            VkRenderPassCreateInfo renderPassCreateInfo = {};
            renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassCreateInfo.attachmentCount = 1;
            renderPassCreateInfo.pAttachments = &colorAttachment;
            renderPassCreateInfo.subpassCount = 1;
            renderPassCreateInfo.pSubpasses = &subpass;
            renderPassCreateInfo.dependencyCount = 1;
            renderPassCreateInfo.pDependencies = &dependency;

            if (vkCreateRenderPass(YzVkDevice::instance()->getDevice(), &renderPassCreateInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
                YZ_ERROR("Vulkan failed to create a render pass.");
                throw std::runtime_error("failed to create render pass!");
            }

        }
        void YzVkRenderPass::cleanUp() {
            if (m_RenderPass) {
                vkDestroyRenderPass(YzVkDevice::instance()->getDevice(), m_RenderPass, nullptr);
            }
        }

		void YzVkRenderPass::beginRenderPass(YzVkCommandBuffer* const commandBuffer, YzVkFramebuffer* const frameBuffer, YzVkSwapchain* const swapchain) {
            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = m_RenderPass;
            renderPassInfo.framebuffer = frameBuffer->getFramebuffer();
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = swapchain->getExtent();
            VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(commandBuffer->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		}
        void YzVkRenderPass::endRenderPass(YzVkCommandBuffer* const commandBuffer) {
            vkCmdEndRenderPass(commandBuffer->getCommandBuffer());
        }
    }
}
