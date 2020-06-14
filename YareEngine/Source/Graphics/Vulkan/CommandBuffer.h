#ifndef YARE_COMMANDBUFFER_H
#define YARE_COMMANDBUFFER_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/CommandPool.h"

namespace Yare::Graphics {
    class YzVkCommandBuffer {
    public:
        YzVkCommandBuffer();
        ~YzVkCommandBuffer();

        void beginRecording();
        void endRecording();

        const VkCommandBuffer& getCommandBuffer() const { return m_CommandBuffer; }
        const VkFence& getFence() const { return m_Fence; }
    private:
        void init();
        VkCommandBuffer m_CommandBuffer;
        VkFence m_Fence = VK_NULL_HANDLE;
    };
}

#endif //YARE_COMMANDBUFFER_H
