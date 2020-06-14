#ifndef YARE_BUFFER_H
#define YARE_BUFFER_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/CommandBuffer.h"
#include "Graphics/Vulkan/Utilities.h"

namespace Yare::Graphics {

    enum class BufferUsage {
                            UNIFORM,
                            DYNAMIC,
                            VERTEX,
                            DYNAMIC_VERTEX,
                            INDEX,
                            DYNAMIC_INDEX,
                            TRANSFER
    };

    class Buffer {
    public:
        Buffer();
        Buffer(BufferUsage usage, size_t size, const void* data);
        ~Buffer();

        void init(BufferUsage usage, size_t size, const void* data);
        void setData(size_t size, const void* data, uint64_t offset = 0);
        void setDynamicData(size_t size, const void* data, uint64_t offset = 0);
        void bindIndex(CommandBuffer* commandBuffer, VkIndexType type);
        void bindVertex(CommandBuffer* commandBuffer, VkDeviceSize offset);
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


#endif //YARE_BUFFER_H
