#include "Graphics/Vulkan/Vk_Buffer.h"
#include "Graphics/Vulkan/Vk_Devices.h"
#include "Utilities/Logger.h"

namespace Yarezo::Graphics {
    YzVkBuffer::YzVkBuffer() {
    }

    YzVkBuffer::YzVkBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, size_t size, const void* data) {
        init(usageFlags, propertyFlags, size, data);
    }

    YzVkBuffer::~YzVkBuffer() {
        if (m_Buffer) {
            vkDestroyBuffer(YzVkDevice::instance()->getDevice(), m_Buffer, nullptr);
            if (m_BufferMemory) {
                vkFreeMemory(YzVkDevice::instance()->getDevice(), m_BufferMemory, nullptr);
            }
        }
    }

    void YzVkBuffer::init(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, size_t size, const void* data) {
        m_Size = size;
        m_Properties = propertyFlags;
        m_Usage = usageFlags;

        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usageFlags;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(YzVkDevice::instance()->getDevice(), &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan was unable to create a buffer.");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(YzVkDevice::instance()->getDevice(), m_Buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VkUtil::findMemoryType(memRequirements.memoryTypeBits, propertyFlags);

        if (vkAllocateMemory(YzVkDevice::instance()->getDevice(), &allocInfo, nullptr, &m_BufferMemory) != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to allocate buffer memory!");
        }

        vkBindBufferMemory(YzVkDevice::instance()->getDevice(), m_Buffer, m_BufferMemory, 0);

        if (data != nullptr) {
            setData(size, data);
        }
    }


    void YzVkBuffer::setData(size_t size, const void* data, uint32_t offset) {
        if (mapMemory(0, size)) {
            auto p = static_cast<char*>(m_MappedData) + offset;
            memcpy((void*)p, data, size);
            unmapMemory();
        }
        else {
            YZ_ERROR("Mapping failed - Not copying data to buffer");
        }
    }

    void YzVkBuffer::setDynamicData(size_t size, const void* data, uint32_t offset) {
        if (mapMemory(0, VK_WHOLE_SIZE)) {
            auto p = static_cast<char*>(m_MappedData) + offset;
            memcpy(p, data, size);
            flush(size, 0);
            unmapMemory();
        }
        else {
            YZ_ERROR("Mapping failed - Not copying data to buffer");
        }
    }

    void YzVkBuffer::bindIndex(YzVkCommandBuffer* commandBuffer, VkIndexType type) {
        // Check that the index buffer bit was set inside the usageflags before binding
        if (m_Usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT) {
            vkCmdBindIndexBuffer(commandBuffer->getCommandBuffer(), m_Buffer, 0, type);
        } else {
            YZ_WARN("Buffer was not of type Index. Did you intend to bind in this way?");
        }

    }

    void YzVkBuffer::bindVertex(YzVkCommandBuffer* commandBuffer, VkDeviceSize offset) {
        // check that the vertex buffer bit was set inside the usageFlags before binding
        if (m_Usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) {
            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(commandBuffer->getCommandBuffer(), 0, 1, &m_Buffer, &offset);
        } else {
            YZ_WARN("Buffer was not of type Vertex. Did you intend to bind in this way?");
        }
    }

    bool YzVkBuffer::mapMemory(VkDeviceSize offset, VkDeviceSize size) {
        if (vkMapMemory(YzVkDevice::instance()->getDevice(), m_BufferMemory, offset, size, 0, &m_MappedData) != VK_SUCCESS) {
            YZ_ERROR("Failed to map buffer memory");
            return false;
        }
        return true;
    }

    void YzVkBuffer::unmapMemory() {
        vkUnmapMemory(YzVkDevice::instance()->getDevice(), m_BufferMemory);
    }

    void YzVkBuffer::flush(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType =  VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = m_BufferMemory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        vkFlushMappedMemoryRanges(YzVkDevice::instance()->getDevice(), 1, &mappedRange);
    }
}
