#ifndef YARE_VK_UTILITIES_H
#define YARE_VK_UTILITIES_H

#include "Graphics/Vulkan/Vk.h"
#include <vector>
#include <string>

namespace Yare::Graphics::VkUtil {
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    std::vector<char> readShaderFile(const std::string& filePath);

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    VkImageView createImageView(VkImage image, VkImageViewType viewType, VkFormat format, uint32_t layerCount, VkImageAspectFlags aspectFlags);

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat();
    bool hasStencilComponent(VkFormat format);
}

#endif //YARE_VK_UTILITIES_H
