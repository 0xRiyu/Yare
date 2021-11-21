#ifndef YARE_COMMANDBUFFER_H
#define YARE_COMMANDBUFFER_H

#include "Graphics/Vulkan/CommandPool.h"
#include "Graphics/Vulkan/Vk.h"

namespace Yare::Graphics {
    class CommandBuffer {
       public:
        CommandBuffer();
        ~CommandBuffer();

        void beginRecording();
        void endRecording();

        const VkCommandBuffer& getCommandBuffer() const { return m_CommandBuffer; }
        const VkFence&         getFence() const { return m_Fence; }

       private:
        void            init();
        VkCommandBuffer m_CommandBuffer;
        VkFence         m_Fence = VK_NULL_HANDLE;
    };
}  // namespace Yare::Graphics

#endif  // YARE_COMMANDBUFFER_H
