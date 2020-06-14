#include "Graphics/Vulkan/Semaphore.h"
#include "Graphics/Vulkan/Devices.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {

    YzVkSemaphore::YzVkSemaphore() {
        init();
    }

    YzVkSemaphore::~YzVkSemaphore() {
        if (m_Semaphore) {
            vkDestroySemaphore(Graphics::YzVkDevice::instance()->getDevice(), m_Semaphore, nullptr);
        }
    }

    void YzVkSemaphore::init() {
        VkSemaphoreCreateInfo sInfo = {};
        sInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        auto res = vkCreateSemaphore(Graphics::YzVkDevice::instance()->getDevice(), &sInfo, nullptr, &m_Semaphore);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to create a semaphore");
        }
    }
}
