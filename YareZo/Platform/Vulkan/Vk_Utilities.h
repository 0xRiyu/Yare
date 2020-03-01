#ifndef Yarezo_VK_UTILITIES_H
#define Yarezo_VK_UTILITIES_H

#include "Platform/Vulkan/Vk.h"

namespace Yarezo {
	namespace Graphics {
		namespace Util {
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		}
	}
}

#endif //Yarezo_VK_UTILITIES_H