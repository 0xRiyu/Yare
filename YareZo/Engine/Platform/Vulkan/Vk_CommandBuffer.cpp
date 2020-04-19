#include "Platform/Vulkan/Vk_Instance.h"
#include "Platform/Vulkan/Vk_CommandBuffer.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Utilities/YzLogger.h"

namespace Yarezo {
	namespace Graphics {
		YzVkCommandBuffer::YzVkCommandBuffer() {

		}


		YzVkCommandBuffer::~YzVkCommandBuffer() {

		}

		void YzVkCommandBuffer::init() {
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = YzVkInstance::getYzVkInstance()->getYzCommandPool().getCommandPool();
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 1;

			if (vkAllocateCommandBuffers(YzVkDevice::instance()->getDevice(), &allocInfo, &m_CommandBuffer) != VK_SUCCESS) {
				YZ_CRITICAL("Vulkan Failed to allocate command buffers.");
			}

			VkFenceCreateInfo fenceInfo = {};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			if (vkCreateFence(YzVkDevice::instance()->getDevice(), &fenceInfo, nullptr, &m_Fence) != VK_SUCCESS) {
				YZ_CRITICAL("Vulkan Failed to create a fence");
			}
			vkResetFences(YzVkDevice::instance()->getDevice(), 1, &m_Fence);

		}

		void YzVkCommandBuffer::cleanUp() {
			if (m_Fence) {
				vkDestroyFence(YzVkDevice::instance()->getDevice(), m_Fence, nullptr);
			}
			if (m_CommandBuffer) {
				vkFreeCommandBuffers(YzVkDevice::instance()->getDevice(), YzVkInstance::getYzVkInstance()->getYzCommandPool().getCommandPool(), 1, &m_CommandBuffer);
			}
		}

		void YzVkCommandBuffer::beginRecording() {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0; // Optional
			beginInfo.pInheritanceInfo = nullptr; // Optional

			if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS) {
				YZ_CRITICAL("Vulkan failed to begin recording command buffer.");
			}
		}

		void YzVkCommandBuffer::endRecording() {
			if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS) {
				YZ_CRITICAL("Vulkan failed to end recording command buffer.");
			}
		}
		void YzVkCommandBuffer::submitGfxQueue(VkPipelineStageFlags flags, VkSemaphore waitSemaphore, VkSemaphore signalSemaphore, bool waitFence) {
			uint32_t waitSemaphoreCount = (waitSemaphore) ? 1 : 0;
			uint32_t signalSemaphoreCount = (signalSemaphore) ? 1 : 0;

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &m_CommandBuffer;
			submitInfo.pWaitDstStageMask = &flags;
			submitInfo.pWaitSemaphores = &waitSemaphore;
			submitInfo.waitSemaphoreCount = waitSemaphoreCount;
			submitInfo.pSignalSemaphores = &signalSemaphore;
			submitInfo.signalSemaphoreCount = signalSemaphoreCount;
			submitInfo.pNext = VK_NULL_HANDLE;

			if (!waitFence) {
				vkQueueSubmit(YzVkDevice::instance()->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
				vkQueueWaitIdle(YzVkDevice::instance()->getGraphicsQueue());
			}
			else {
				vkQueueSubmit(YzVkDevice::instance()->getGraphicsQueue(), 1, &submitInfo, m_Fence);
				vkWaitForFences(YzVkDevice::instance()->getDevice(), 1, &m_Fence, VK_TRUE, UINT64_MAX);
				vkResetFences(YzVkDevice::instance()->getDevice(), 1, &m_Fence);
			}
		}
	}
}
