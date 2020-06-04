#ifndef YAREZO_VK_IMAGE_H
#define YAREZO_VK_IMAGE_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/Vk_Buffer.h"

#include <string>

namespace Yarezo::Graphics {
    class YzVkImage {
    protected:
        // Only allow the static constructors
        YzVkImage() {}

    public:
        ~YzVkImage();

        void createTexture2DFromFile(const std::string& filePath);
        void createTextureCubeFromFile(const std::string& filePath);
        void createTextureCubeFromFiles(const std::vector<std::string>& filePaths);
        void createEmptyTexture(size_t width, size_t height, VkFormat format,
                                VkImageTiling tiling, VkImageUsageFlags usage,
                                VkMemoryPropertyFlags properties, VkImageAspectFlagBits flagBits);
        void createTexture2DFromData(size_t width, size_t height, VkFormat format, unsigned char* data);

        const VkImage&         getImage()     const { return m_Image; }
        const VkDeviceMemory&  getMemory()    const { return m_ImageMemory; }
        const VkImageView&     getImageView() const { return m_ImageView; }
        const VkSampler&       getSampler()   const { return m_Sampler; }

    private:
        void loadTextureFromFileIntoBuffer(const std::string& filePath, YzVkBuffer& buffer);
        void loadTexturesFromFilesIntoBuffer(const std::vector<std::string>& filePaths, YzVkBuffer& buffer);
        void createTexture2D(const YzVkBuffer& buffer, VkFormat format);
        void createTextureCube(const YzVkBuffer& buffer);
        void transitionImageLayout(VkFormat format, uint32_t layerCount, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(const YzVkBuffer& buffer, uint32_t faces, uint32_t mipLevels);

        void createImage(VkImageType type, VkFormat format, VkImageTiling tiling,
                         VkImageUsageFlags usage, VkImageCreateFlags flags,
                         VkMemoryPropertyFlags properties);
        void createSampler(VkSamplerAddressMode mode);

        VkImage         m_Image       = VK_NULL_HANDLE;
        VkDeviceMemory  m_ImageMemory = VK_NULL_HANDLE;
        VkImageView     m_ImageView   = VK_NULL_HANDLE;
        VkSampler       m_Sampler     = VK_NULL_HANDLE;

        size_t m_TextureWidth = 0;
        size_t m_TextureHeight = 0;
        size_t m_TextureChannels = 0;

    public:
        static YzVkImage* createDepthStencilBuffer(size_t width, size_t height, VkFormat format);
        static YzVkImage* createTexture2D(size_t width, size_t height, VkFormat format, unsigned char* data);
        static YzVkImage* createTexture2D(const std::string& filePath);
        static YzVkImage* createTextureCube(const std::vector<std::string>& filePaths);
    };
}

#endif // YAREZO_VK_IMAGE_H
