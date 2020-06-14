#include "Graphics/Vulkan/Buffer.h"
#include "Graphics/Vulkan/Devices.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {
    YzVkBuffer::YzVkBuffer() {
    }

    YzVkBuffer::YzVkBuffer(BufferUsage usage, size_t size, const void* data) {
        init(usage, size, data);
    }

    YzVkBuffer::~YzVkBuffer() {
        if (m_Buffer) {
            vkDestroyBuffer(YzVkDevice::instance()->getDevice(), m_Buffer, nullptr);
            if (m_BufferMemory) {
                vkFreeMemory(YzVkDevice::instance()->getDevice(), m_BufferMemory, nullptr);
            }
        }
    }

    void YzVkBuffer::init(BufferUsage usage, size_t size, const void* data) {
        m_Size = size;
        m_Usage = usage;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags propFlags;

        switch(usage) {
        case BufferUsage::UNIFORM:
            usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            propFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            break;
        case BufferUsage::DYNAMIC:
            usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            propFlags =  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
            break;
        case BufferUsage::VERTEX:
            usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            propFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            break;
        case BufferUsage::DYNAMIC_VERTEX:
            usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            propFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
            break;
        case BufferUsage::INDEX:
            usageFlags =  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            propFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            break;
        case BufferUsage::DYNAMIC_INDEX:
            usageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            propFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
            break;
        case BufferUsage::TRANSFER:
            usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            propFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            break;
        }

        createBuffer(usageFlags, propFlags);

        if (data != nullptr) {
            setData(size, data);
        }
    }

    void YzVkBuffer::setData(size_t size, const void* data, uint64_t offset) {
        if (mapMemory(size, 0)) {
            auto p = static_cast<char*>(m_MappedData) + offset;
            memcpy((void*)p, data, size);
            unmapMemory();
        }
        else {
            YZ_ERROR("Mapping failed - Not copying data to buffer");
        }
    }

    void YzVkBuffer::setDynamicData(size_t size, const void* data, uint64_t offset) {
        if (mapMemory(VK_WHOLE_SIZE, 0)) {
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
        if (m_Usage == BufferUsage::INDEX || m_Usage == BufferUsage::DYNAMIC_INDEX) {
            vkCmdBindIndexBuffer(commandBuffer->getCommandBuffer(), m_Buffer, 0, type);
        } else {
            YZ_WARN("Buffer was not of type Index. Did you intend to bind in this way?");
        }

    }

    void YzVkBuffer::bindVertex(YzVkCommandBuffer* commandBuffer, VkDeviceSize offset) {
        // check that the vertex buffer bit was set inside the usageFlags before binding
        if (m_Usage == BufferUsage::VERTEX || m_Usage == BufferUsage::DYNAMIC_VERTEX) {
            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(commandBuffer->getCommandBuffer(), 0, 1, &m_Buffer, &offset);
        } else {
            YZ_WARN("Buffer was not of type Vertex. Did you intend to bind in this way?");
        }
    }

    bool YzVkBuffer::mapMemory(VkDeviceSize size, VkDeviceSize offset) {
        auto res = vkMapMemory(YzVkDevice::instance()->getDevice(), m_BufferMemory, offset, size, 0, &m_MappedData);
        if (res != VK_SUCCESS) {
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

    void YzVkBuffer::createBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags props) {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = m_Size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        auto res = vkCreateBuffer(YzVkDevice::instance()->getDevice(), &bufferInfo, nullptr, &m_Buffer);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan was unable to create a buffer.");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(YzVkDevice::instance()->getDevice(), m_Buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VkUtil::findMemoryType(memRequirements.memoryTypeBits, props);

        res = vkAllocateMemory(YzVkDevice::instance()->getDevice(), &allocInfo, nullptr, &m_BufferMemory);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to allocate buffer memory!");
        }

        vkBindBufferMemory(YzVkDevice::instance()->getDevice(), m_Buffer, m_BufferMemory, 0);
    }
}
