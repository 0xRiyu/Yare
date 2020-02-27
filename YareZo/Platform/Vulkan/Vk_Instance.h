#ifndef YZ_VK_INSTANCE_HPP
#define YZ_VK_INSTANCE_HPP

#include "Platform/Vulkan/Vk.h"
#include <vector>

namespace Yarezo {
    namespace Graphics {

        class YzVkInstance {
        public:
            YzVkInstance();
            ~YzVkInstance();
            void init();
            const VkInstance& getVKInstance() const { return m_Instance; }
            static inline YzVkInstance* getYzVkInstancePtr(){ return s_YzVkInstance; }

        private:
            void createInstance();
            void setupDebugMessenger();
            void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            std::vector<const char*> getRequiredExtensions();
            bool checkValidationLayerSupport();

        private:
            VkInstance m_Instance = VK_NULL_HANDLE;
            VkDebugUtilsMessengerEXT m_DebugMessenger;

            const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
            };

            #ifdef NDEBUG
                const bool enableValidationLayers = false;
            #else
                const bool enableValidationLayers = true;
            #endif

            static YzVkInstance* s_YzVkInstance;
        };
    }
}

#endif // YZ_VK_INSTANCE_HPP