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
			allocInfo.commandPool = YzVkInstance::getYzVkInstancePtr()->getYzCommandPool().getCommandPool();
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 1;

			if (vkAllocateCommandBuffers(YzVkDevice::instance()->getDevice(), &allocInfo, &m_CommandBuffer) != VK_SUCCESS) {
				YZ_CRITICAL("Vulkan Failed to allocate command buffers.");
			}
		
		}

		void YzVkCommandBuffer::cleanUp() {
			if (m_CommandBuffer) {
				vkFreeCommandBuffers(YzVkDevice::instance()->getDevice(), YzVkInstance::getYzVkInstancePtr()->getYzCommandPool().getCommandPool(), 1, &m_CommandBuffer);
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
	}
}
