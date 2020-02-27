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

            const VkSwapchainKHR& getSwapchain()            const { return m_Swapchain; }
            const size_t getImagesSize()                   const { return m_SwapchainImages.size(); }
            const size_t getImageViewSize()                const { return m_SwapchainImageViews.size(); }
            const VkImage& getImage(uint32_t index)         const { return m_SwapchainImages[index]; }
            const VkImageView& getImageView(uint32_t index) const { return m_SwapchainImageViews[index]; }
            const VkFormat& getImageFormat()                const { return m_SwapchainImageFormat; }
            const VkExtent2D& getExtent()                   const { return m_SwapchainExtent; }

        private:
            void createSwapchain();
            void createImageViews();
            VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
            VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
            VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        private:
            VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
            VkFormat m_SwapchainImageFormat;
            VkExtent2D m_SwapchainExtent;
            std::vector<VkImage> m_SwapchainImages;
            std::vector<VkImageView> m_SwapchainImageViews;
        };
    }
}

#endif //YAREZO_VK_SWAPCHAIN_H