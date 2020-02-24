#ifndef YAREZO_VK_SWAPCHAIN_H
#define YAREZO_VK_SWAPCHAIN_H

#include "Platform/Vulkan/Vk.h"

#include <vector>

namespace Yarezo {
    namespace Graphics {
        class YzVkSwapchain {
        public:
            YzVkSwapchain();
            ~YzVkSwapchain();

            void init();
            void cleanUp();

            inline VkSwapchainKHR getSwapchain()        const { return m_Swapchain; }
            inline size_t getImagesSize()               const { return m_SwapchainImages.size(); }
            inline VkImage getImage(uint32_t index)     const { return m_SwapchainImages[index]; }
            inline VkFormat getImageFormat()            const { return m_SwapchainImageFormat; }
            inline VkExtent2D getExtent()               const { return m_SwapchainExtent; }

        private:
            void createSwapchain();
            VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
            VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
            VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        private:
            VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
            std::vector<VkImage> m_SwapchainImages;
            VkFormat m_SwapchainImageFormat;
            VkExtent2D m_SwapchainExtent;
        };
    }
}

#endif //YAREZO_VK_SWAPCHAIN_H