#ifndef YARE_SWAPCHAIN_H
#define YARE_SWAPCHAIN_H

#include <vector>

#include "Graphics/Vulkan/Vk.h"

namespace Yare::Graphics {
    class Swapchain {
       public:
        Swapchain(size_t width, size_t height);
        ~Swapchain();

        VkResult present(VkSemaphore waitSemaphore);
        VkResult acquireNextImage(VkSemaphore signalSemaphore);
        void     onResize(size_t width, size_t height);

        const VkSwapchainKHR& getSwapchain() const { return m_Swapchain; }
        const size_t          getImagesSize() const { return m_SwapchainImages.size(); }
        const size_t          getImageViewSize() const { return m_SwapchainImageViews.size(); }
        const VkImage&        getImage(uint32_t index) const { return m_SwapchainImages[index]; }
        const VkImageView&    getImageView(uint32_t index) const { return m_SwapchainImageViews[index]; }
        const VkFormat&       getImageFormat() const { return m_SwapchainImageFormat; }
        const VkExtent2D&     getExtent() const { return m_SwapchainExtent; }
        const uint32_t        getCurrentImage() const { return m_CurrentImage; }

       private:
        void               init(size_t width, size_t height);
        void               createSwapchain(size_t width, size_t height);
        void               createImageViews();
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR   chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D         chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, size_t width, size_t height);

       private:
        VkSwapchainKHR           m_Swapchain = VK_NULL_HANDLE;
        VkFormat                 m_SwapchainImageFormat;
        VkExtent2D               m_SwapchainExtent;
        std::vector<VkImage>     m_SwapchainImages;
        std::vector<VkImageView> m_SwapchainImageViews;
        uint32_t                 m_CurrentImage = 0;
    };
}  // namespace Yare::Graphics

#endif  // YARE_SWAPCHAIN_H
