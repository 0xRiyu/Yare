#include "Platform/Vulkan/Vk_Utilities.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Utilities/YzLogger.h"

#include <fstream>

namespace Yarezo::Graphics::VkUtil {

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(YzVkDevice::instance()->getGPU(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        YZ_ERROR("Vulkan failed to find a suitable memory type.");
        return 0;
    }


    std::vector<char> readShaderFile(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            YZ_ERROR("File '" + filePath + "' was unable to open.");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;

    }

    VkCommandBuffer beginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = YzVkDevice::instance()->getYzVkInstance()->getYzCommandPool().getCommandPool();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(YzVkDevice::instance()->getDevice(), &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(YzVkDevice::instance()->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(YzVkDevice::instance()->getGraphicsQueue());

        vkFreeCommandBuffers(YzVkDevice::instance()->getDevice(), YzVkDevice::instance()->getYzVkInstance()->getYzCommandPool().getCommandPool(), 1, &commandBuffer);
    }

    VkImageView createImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags) {
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = viewType;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(YzVkDevice::instance()->getDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            YZ_CRITICAL("failed to create an image view!");
        }

        return imageView;
    }

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(YzVkDevice::instance()->getGPU(), format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            }

            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }
        YZ_CRITICAL("failed to find a supported format.");
    }

    VkFormat findDepthFormat() {
        return findSupportedFormat(
                                   { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
                                   VK_IMAGE_TILING_OPTIMAL,
                                   VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
                                   );
    }

    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

}
