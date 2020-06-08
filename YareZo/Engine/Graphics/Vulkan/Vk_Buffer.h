#ifndef YAREZO_VK_BUFFER_H
#define YAREZO_VK_BUFFER_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/Vk_CommandBuffer.h"
#include "Graphics/Vulkan/Vk_Utilities.h"

namespace Yarezo::Graphics {

    enum class BufferUsage {
                            UNIFORM,
                            DYNAMIC,
                            VERTEX,
                            DYNAMIC_VERTEX,
                            INDEX,
                            DYNAMIC_INDEX,
                            TRANSFER
    };

    class YzVkBuffer {
    public:
        YzVkBuffer();
        YzVkBuffer(BufferUsage usage, size_t size, const void* data);
        ~YzVkBuffer();

        void init(BufferUsage usage, size_t size, const void* data);
        void setData(size_t size, const void* data, uint64_t offset = 0);
        void setDynamicData(size_t size, const void* data, uint64_t offset = 0);
        void bindIndex(YzVkCommandBuffer* commandBuffer, VkIndexType type);
        void bindVertex(YzVkCommandBuffer* commandBuffer, VkDeviceSize offset);
        void flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        bool mapMemory(VkDeviceSize size, VkDeviceSize offset);
        void unmapMemory();

        const VkBuffer& getBuffer() const { return m_Buffer; }
        void* getMappedData() const { return m_MappedData; }
        size_t getSize() const { return m_Size; }

    private:
        void createBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags props);

        VkBuffer m_Buffer;
        BufferUsage  m_Usage;
        VkDeviceMemory m_BufferMemory;
        size_t m_Size = 0;
        void* m_MappedData = nullptr;
    };
}


#endif //YAREZO_VK_BUFFER_H
