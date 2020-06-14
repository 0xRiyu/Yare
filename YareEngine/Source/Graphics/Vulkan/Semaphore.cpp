#include "Graphics/Vulkan/Semaphore.h"
#include "Graphics/Vulkan/Devices.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {

    Semaphore::Semaphore() {
        init();
    }

    Semaphore::~Semaphore() {
        if (m_Semaphore) {
            vkDestroySemaphore(Devices::instance()->getDevice(), m_Semaphore, nullptr);
        }
    }

    void Semaphore::init() {
        VkSemaphoreCreateInfo sInfo = {};
        sInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        auto res = vkCreateSemaphore(Devices::instance()->getDevice(), &sInfo, nullptr, &m_Semaphore);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to create a semaphore");
        }
    }
}
