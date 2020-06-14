#ifndef YARE_INSTANCE_H
#define YARE_INSTANCE_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/CommandPool.h"

#include <vector>

namespace Yare::Graphics {
    class YzVkInstance {
    public:
        YzVkInstance();
        ~YzVkInstance();
        void init();
        void cleanUp();
        void createCommandPool();

        const VkInstance& getVKInstance()              const { return m_Instance; }
        const YzVkCommandPool* getYzCommandPool()      const { return m_YzCommandPool; }
        static inline YzVkInstance* getYzVkInstance()        { return s_YzVkInstance; }

    private:
        void createInstance();
        void setupDebugMessenger();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();

        VkInstance m_Instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_DebugMessenger;

        // To be initialized after a device has been created
        YzVkCommandPool* m_YzCommandPool;

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

#endif // YARE_INSTANCE_H
