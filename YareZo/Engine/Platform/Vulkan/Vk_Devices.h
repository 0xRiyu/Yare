#ifndef VK_DEVICES_HPP
#define VK_DEVICES_HPP

#include "Utilities/T_Singleton.h"
#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_Instance.h"

#include <vector>

namespace Yarezo {
    namespace Graphics {

        struct QueueFamilyIndices {
            int graphicsFamily = -1;
            int presentFamily = -1;

            bool isComplete() {
                return graphicsFamily >= 0 && presentFamily >= 0;
            }
        };

        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        class YzVkDevice : public Utilities::T_Singleton<YzVkDevice> {

        public:
            YzVkDevice();
            ~YzVkDevice();
            void init();

            void waitIdle();

            const VkSurfaceKHR& getSurfaceKHR()     const { return m_Surface; }
            const VkDevice& getDevice()             const { return m_Device; }
            const VkPhysicalDevice& getGPU()        const { return m_PhysicalDevice; }
            const VkQueue& getGraphicsQueue()       const { return m_GraphicsQueue; }
            const VkQueue& getPresentQueue()        const { return m_PresentQueue; }
            const YzVkInstance* getYzVkInstance()   const { return m_YzVkInstance; }

            QueueFamilyIndices getQueueFamilyIndicies();
            SwapChainSupportDetails getSwapChainSupport();

        private:
            void createSurface();
            void pickPhysicalDevice();
            void createLogicalDevice();
            bool isDeviceSuitable(VkPhysicalDevice device);
            bool checkDeviceExtensionSupport(VkPhysicalDevice device);
            QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
            SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        private:
            VkSurfaceKHR m_Surface              = VK_NULL_HANDLE;
            VkDevice m_Device                   = VK_NULL_HANDLE;
            VkPhysicalDevice m_PhysicalDevice   = VK_NULL_HANDLE;
            VkQueue m_GraphicsQueue             = VK_NULL_HANDLE;
            VkQueue m_PresentQueue              = VK_NULL_HANDLE;

            YzVkInstance* m_YzVkInstance;

            const std::vector<const char*> m_DeviceExtensions{
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };
        };
    }
}

#endif //VK_DEVICES_HPP