#include "Platform/Vulkan/Vk_Utilities.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Utilities/YzLogger.h"

#include <fstream>

namespace Yarezo {
	namespace Graphics {
		namespace VkUtil {
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
                VkPhysicalDeviceMemoryProperties memProperties;
                vkGetPhysicalDeviceMemoryProperties(YzVkDevice::instance()->getGPU(), &memProperties);

                for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                        return i;
                    }
                }
                YZ_ERROR("Vulkan failed to find a suitable memory type.");
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
		}
	}
}