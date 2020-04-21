#ifndef YZ_VK_INSTANCE_HPP
#define YZ_VK_INSTANCE_HPP

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_CommandPool.h"

#include <vector>

namespace Yarezo {
    namespace Graphics {

        class YzVkInstance {
        public:
            YzVkInstance();
            ~YzVkInstance();
            void init();
            void cleanUp();
            void createCommandPool();
            const VkInstance& getVKInstance() const { return m_Instance; }
            const YzVkCommandPool& getYzCommandPool() const { return m_YzCommandPool; }
            static inline YzVkInstance* getYzVkInstance(){ return s_YzVkInstance; }

        private:
            void createInstance();
            void setupDebugMessenger();
            void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            std::vector<const char*> getRequiredExtensions();
            bool checkValidationLayerSupport();

        private:
            VkInstance m_Instance = VK_NULL_HANDLE;
            VkDebugUtilsMessengerEXT m_DebugMessenger;
            // To be initialized after a device has been created
            YzVkCommandPool m_YzCommandPool;

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
