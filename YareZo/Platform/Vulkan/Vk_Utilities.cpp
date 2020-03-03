#include "Platform/Vulkan/Vk_Utilities.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Utilities/YzLogger.h"

namespace Yarezo {
	namespace Graphics {
		namespace Util {
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
                VkPhysicalDeviceMemoryProperties memProperties;
                vkGetPhysicalDeviceMemoryProperties(YzVkDevice::instance()->getGPU(), &memProperties);

                for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                        return i;
                    }
                }
                YZ_ERROR("Vulkan failed to find a suitable memory type.");
                throw std::runtime_error("Vulkan failed to find a suitable memory type.");
			}

			VkShaderModule createShaderModule(const std::vector<char>& shader_code) {
                VkShaderModuleCreateInfo createInfo = {};
                createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                createInfo.codeSize = shader_code.size();
                createInfo.pCode = reinterpret_cast<const uint32_t*>(shader_code.data());

                VkShaderModule shaderModule;
                if (vkCreateShaderModule(YzVkDevice::instance()->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
                    YZ_ERROR("Vulkan was unable to create a shaderModule with provided shader code.");
                    throw std::runtime_error("Vulkan was unable to create a shaderModule with provided shader code.");
                }

                return shaderModule;
			}
		}
	}
}