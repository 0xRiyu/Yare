#ifndef YARE_VK_UTILITIES_H
#define YARE_VK_UTILITIES_H

#include <string>
#include <vector>

#include "Graphics/Vulkan/Vk.h"

namespace Yare::Graphics::VkUtil {
    uint32_t          findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    std::vector<char> readShaderFile(const std::string& filePath);

    VkCommandBuffer beginSingleTimeCommands();
    void            endSingleTimeCommands(VkCommandBuffer commandBuffer);

    VkImageView createImageView(VkImage image, VkImageViewType viewType, VkFormat format, uint32_t layerCount,
                                VkImageAspectFlags aspectFlags);

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
                                 VkFormatFeatureFlags features);
    VkFormat findDepthFormat();
    bool     hasStencilComponent(VkFormat format);
}  // namespace Yare::Graphics::VkUtil

#endif  // YARE_VK_UTILITIES_H
