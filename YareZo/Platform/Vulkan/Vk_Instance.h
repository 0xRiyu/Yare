#ifndef YZ_VK_INSTANCE_HPP
#define YZ_VK_INSTANCE_HPP

#include "Platform/Vulkan/Vk.h"
#include "src/Window.h"

#include <vector>

namespace Yarezo {
    namespace Graphics {

        class YzVkInstance {
        public:
            YzVkInstance();
            ~YzVkInstance();
            void Init();
            inline VkInstance GetVKInstance() const { return m_Instance; }

        private:
            void CreateInstance();
            void setupDebugMessenger();
            void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            std::vector<const char*> getRequiredExtensions();
            bool checkValidationLayerSupport();

        private:
            VkInstance m_Instance = nullptr;
            VkDebugUtilsMessengerEXT m_DebugMessenger;

            const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
            };

            #ifdef NDEBUG
                const bool enableValidationLayers = false;
            #else
                const bool enableValidationLayers = true;
            #endif
        };
    }
}

#endif // YZ_VK_INSTANCE_HPP