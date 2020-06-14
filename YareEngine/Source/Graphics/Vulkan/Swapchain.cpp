#include "Graphics/Vulkan/Swapchain.h"
#include "Graphics/Vulkan/Devices.h"
#include "Graphics/Vulkan/Utilities.h"
#include "Core/Yzh.h"

namespace Yare::Graphics {

    Swapchain::Swapchain(size_t width, size_t height) {
        init(width, height);
    }

    Swapchain::~Swapchain() {
        for (auto& imageView : m_SwapchainImageViews) {
            vkDestroyImageView(Devices::instance()->getDevice(), imageView, nullptr);
        }

        if (m_Swapchain) {
            vkDestroySwapchainKHR(Devices::instance()->getDevice(), m_Swapchain, nullptr);
        }
    }

    void Swapchain::onResize(size_t width, size_t height) {
        //Cleanup
        {
            for (auto& imageView : m_SwapchainImageViews) {
                vkDestroyImageView(Devices::instance()->getDevice(), imageView, nullptr);
            }
        }
        init(width, height);
    }

    void Swapchain::init(size_t width, size_t height) {
        // Create a swapchain, a swapchain is responsible for maintaining the images
        // that will be presented to the user.
        createSwapchain(width, height);
        // Create and image-view, which will represent a 'view' of an image,
        // this way we can interface with images without modifying the underlying image
        createImageViews();
    }

    VkResult Swapchain::present(VkSemaphore waitSemaphore) {
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &waitSemaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_Swapchain;
        presentInfo.pImageIndices = &m_CurrentImage;
        presentInfo.pResults = nullptr; // Optional
        return vkQueuePresentKHR(Devices::instance()->getPresentQueue(), &presentInfo);
    }

    VkResult Swapchain::acquireNextImage(VkSemaphore signalSemaphore) {
        return vkAcquireNextImageKHR(Devices::instance()->getDevice(), m_Swapchain, UINT64_MAX,
                                     signalSemaphore, VK_NULL_HANDLE, &m_CurrentImage);
    }

    void Swapchain::createSwapchain(size_t width, size_t height) {
        auto YzVkDeviceinstance = Devices::instance();

        SwapChainSupportDetails swapChainSupport = YzVkDeviceinstance->getSwapChainSupport();

        auto surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        auto presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        auto extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

        // Request one more image to render to than the minimum so we dont wait for driver operations
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if (swapChainSupport.capabilities.maxImageCount > 0 &&
                imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = YzVkDeviceinstance->getSurfaceKHR();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        // In the event that this creation function is being called twice,
        // We need to 'retire' the old swapchain
        if (m_Swapchain) {
            createInfo.oldSwapchain = m_Swapchain;
        } else {
            createInfo.oldSwapchain = VK_NULL_HANDLE;
        }

        Graphics::QueueFamilyIndices indices = YzVkDeviceinstance->getQueueFamilyIndicies();
        uint32_t queueFamilyIndices[] = { static_cast<uint32_t>(indices.graphicsFamily),
                                          static_cast<uint32_t>(indices.presentFamily) };

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        auto res = vkCreateSwapchainKHR(YzVkDeviceinstance->getDevice(), &createInfo, nullptr, &m_Swapchain);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("SwapChain failed to create");
        }

        vkGetSwapchainImagesKHR(YzVkDeviceinstance->getDevice(), m_Swapchain, &imageCount, nullptr);
        m_SwapchainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(YzVkDeviceinstance->getDevice(), m_Swapchain, &imageCount, m_SwapchainImages.data());

        m_SwapchainImageFormat = surfaceFormat.format;
        m_SwapchainExtent = extent;
    }

    void Swapchain::createImageViews() {
        m_SwapchainImageViews.resize(getImagesSize());

        for (uint32_t i = 0; i < getImagesSize(); i++) {
            m_SwapchainImageViews[i] = VkUtil::createImageView(m_SwapchainImages[i], VK_IMAGE_VIEW_TYPE_2D,
                                                               m_SwapchainImageFormat, 1, VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }

    VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }
        return availableFormats[0];
    }

    VkPresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR ||
                availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                return availablePresentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D Swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
                                               size_t width, size_t height) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }
        else {
            VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

            actualExtent.width = std::max(capabilities.minImageExtent.width,
                                          std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height,
                                           std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }
}
