#include "Graphics/Vulkan/CommandPool.h"

#include "Graphics/Vulkan/Devices.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {

    CommandPool::CommandPool() { init(); }

    CommandPool::~CommandPool() {
        if (m_CommandPool) {
            vkDestroyCommandPool(Devices::instance()->getDevice(), m_CommandPool, nullptr);
        }
    }

    void CommandPool::init() {
        Graphics::QueueFamilyIndices queueFamilyIndices = Devices::instance()->getQueueFamilyIndicies();

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;  // Optional

        auto res = vkCreateCommandPool(Devices::instance()->getDevice(), &poolInfo, nullptr, &m_CommandPool);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to create a command pool.");
        }
    }

}  // namespace Yare::Graphics
