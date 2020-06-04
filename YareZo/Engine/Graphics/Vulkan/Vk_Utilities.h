#ifndef Yarezo_VK_UTILITIES_H
#define Yarezo_VK_UTILITIES_H

#include "Graphics/Vulkan/Vk.h"
#include <vector>
#include <string>

namespace Yarezo::Graphics::VkUtil {
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    std::vector<char> readShaderFile(const std::string& filePath);

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    VkImageView createImageView(VkImage image, VkImageViewType viewType, VkFormat format, uint32_t layerCount, VkImageAspectFlags aspectFlags);

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat();
    bool hasStencilComponent(VkFormat format);
}

#endif //Yarezo_VK_UTILITIES_H
