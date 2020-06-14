#include "Graphics/Vulkan/CommandPool.h"
#include "Graphics/Vulkan/Devices.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {

    YzVkCommandPool::YzVkCommandPool() {
        init();
    }

    YzVkCommandPool::~YzVkCommandPool() {
        if (m_CommandPool) {
            vkDestroyCommandPool(YzVkDevice::instance()->getDevice(), m_CommandPool, nullptr);
        }
    }

    void YzVkCommandPool::init() {
        Graphics::QueueFamilyIndices queueFamilyIndices = YzVkDevice::instance()->getQueueFamilyIndicies();

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

        auto res = vkCreateCommandPool(YzVkDevice::instance()->getDevice(), &poolInfo, nullptr, &m_CommandPool);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to create a command pool.");
        }
    }

}
