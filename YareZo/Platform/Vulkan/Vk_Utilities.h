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

		}
	}
}

#endif //Yarezo_VK_UTILITIES_H