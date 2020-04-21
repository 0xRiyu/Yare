#include "Platform/Vulkan/Vk_Semaphore.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Utilities/YzLogger.h"

namespace Yarezo {
	namespace Graphics {
		YzVkSemaphore::YzVkSemaphore() {
		}

		YzVkSemaphore::~YzVkSemaphore() {
		}

		void YzVkSemaphore::init() {
			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(Graphics::YzVkDevice::instance()->getDevice(), &semaphoreInfo, nullptr, &m_Semaphore) != VK_SUCCESS) {
				YZ_CRITICAL("Vulkan failed to create a semaphore");
			}
		}

		void YzVkSemaphore::cleanUp() {
			if (m_Semaphore) {
				vkDestroySemaphore(Graphics::YzVkDevice::instance()->getDevice(), m_Semaphore, nullptr);
			}
		}
	}
}
