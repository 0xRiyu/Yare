#include "Graphics/Vulkan/Vk_CommandBuffer.h"
#include "Graphics/Vulkan/Vk_Instance.h"
#include "Graphics/Vulkan/Vk_Devices.h"
#include "Utilities/Logger.h"

namespace Yarezo::Graphics {

        YzVkCommandBuffer::YzVkCommandBuffer() {
            init();
        }

        YzVkCommandBuffer::~YzVkCommandBuffer() {
            if (m_Fence) {
                 vkDestroyFence(YzVkDevice::instance()->getDevice(), m_Fence, nullptr);
            }
            if (m_CommandBuffer) {
                vkFreeCommandBuffers(YzVkDevice::instance()->getDevice(),
                                     YzVkInstance::getYzVkInstance()->getYzCommandPool()->getCommandPool(),
                                     1, &m_CommandBuffer);
            }
        }

        void YzVkCommandBuffer::init() {
            VkCommandBufferAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = YzVkInstance::getYzVkInstance()->getYzCommandPool()->getCommandPool();
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;

            auto res = vkAllocateCommandBuffers(YzVkDevice::instance()->getDevice(), &allocInfo, &m_CommandBuffer);
            if (res != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan Failed to allocate command buffers.");
            }

            VkFenceCreateInfo fenceInfo = {};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            res = vkCreateFence(YzVkDevice::instance()->getDevice(), &fenceInfo, nullptr, &m_Fence);
            if (res != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan Failed to create a fence");
            }
            vkResetFences(YzVkDevice::instance()->getDevice(), 1, &m_Fence);
        }

    void YzVkCommandBuffer::beginRecording() {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        auto res = vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to begin recording command buffer.");
        }
    }

    void YzVkCommandBuffer::endRecording() {
        auto res = vkEndCommandBuffer(m_CommandBuffer);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to end recording command buffer.");
        }
    }
}
