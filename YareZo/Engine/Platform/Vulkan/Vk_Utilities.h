#ifndef Yarezo_VK_UTILITIES_H
#define Yarezo_VK_UTILITIES_H

#include "Platform/Vulkan/Vk.h"
#include <vector>
#include <string>

namespace Yarezo {
	namespace Graphics {
		namespace VkUtil {
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
			std::vector<char> readShaderFile(const std::string& filePath);

			VkCommandBuffer beginSingleTimeCommands();
			void endSingleTimeCommands(VkCommandBuffer commandBuffer);
			void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

			VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

			VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
			VkFormat findDepthFormat();
			bool hasStencilComponent(VkFormat format);
			void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		}
	}
}

#endif //Yarezo_VK_UTILITIES_H