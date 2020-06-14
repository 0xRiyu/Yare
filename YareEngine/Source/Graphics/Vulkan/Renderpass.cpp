#include "Graphics/Vulkan/Renderpass.h"
#include "Graphics/Vulkan/Devices.h"
#include "Graphics/Vulkan/Framebuffer.h"
#include "Graphics/Vulkan/Utilities.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {

    RenderPass::RenderPass(const RenderPassInfo& info)
        :m_Info(info) {
        init();
    }

    RenderPass::~RenderPass() {
        if (m_RenderPass) {
            vkDestroyRenderPass(Devices::instance()->getDevice(), m_RenderPass, nullptr);
        }
    }

    void RenderPass::init() {
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = m_Info.imageFormat;
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

        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = VkUtil::findDepthFormat();
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef = {};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
        VkRenderPassCreateInfo rpCreateInfo = {};
        rpCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        rpCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        rpCreateInfo.pAttachments = attachments.data();
        rpCreateInfo.subpassCount = 1;
        rpCreateInfo.pSubpasses = &subpass;
        rpCreateInfo.dependencyCount = 1;
        rpCreateInfo.pDependencies = &dependency;

        auto res = vkCreateRenderPass(Devices::instance()->getDevice(), &rpCreateInfo, nullptr, &m_RenderPass);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to create a render pass.");
        }
    }

    void RenderPass::beginRenderPass(const CommandBuffer* commandBuffer, const Framebuffer* frameBuffer) {
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_RenderPass;
        renderPassInfo.framebuffer = frameBuffer->getFramebuffer();
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_Info.extent;
        std::array<VkClearValue, 2> clearValues = {};
        clearValues[0].color = { 0.7f, 0.8f, 0.9f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void RenderPass::endRenderPass(const CommandBuffer* commandBuffer) {
        vkCmdEndRenderPass(commandBuffer->getCommandBuffer());
    }
}
