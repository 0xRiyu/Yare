#include "Graphics/Vulkan/CommandBuffer.h"

#include "Graphics/Vulkan/Context.h"
#include "Graphics/Vulkan/Devices.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {

    CommandBuffer::CommandBuffer() { init(); }

    CommandBuffer::~CommandBuffer() {
        if (m_Fence) {
            vkDestroyFence(Devices::instance()->getDevice(), m_Fence, nullptr);
        }
        if (m_CommandBuffer) {
            vkFreeCommandBuffers(Devices::instance()->getDevice(),
                                 VulkanContext::getContext()->getCommandPool()->getPool(), 1, &m_CommandBuffer);
        }
    }

    void CommandBuffer::init() {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = VulkanContext::getContext()->getCommandPool()->getPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        auto res = vkAllocateCommandBuffers(Devices::instance()->getDevice(), &allocInfo, &m_CommandBuffer);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan Failed to allocate command buffers.");
        }

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        res = vkCreateFence(Devices::instance()->getDevice(), &fenceInfo, nullptr, &m_Fence);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan Failed to create a fence");
        }
        vkResetFences(Devices::instance()->getDevice(), 1, &m_Fence);
    }

    void CommandBuffer::beginRecording() {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;  // Optional
        beginInfo.pInheritanceInfo = nullptr;                           // Optional

        auto res = vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to begin recording command buffer.");
        }
    }

    void CommandBuffer::endRecording() {
        auto res = vkEndCommandBuffer(m_CommandBuffer);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to end recording command buffer.");
        }
    }
}  // namespace Yare::Graphics
