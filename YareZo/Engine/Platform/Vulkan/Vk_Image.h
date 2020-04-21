#ifndef YAREZO_VK_IMAGE_H
#define YAREZO_VK_IMAGE_H

#include "Platform/Vulkan/Vk.h"
#include <string>

namespace Yarezo {
	namespace Graphics {

		class YzVkSampler {
		public:
			YzVkSampler();
			~YzVkSampler();

			void init();
			void cleanUp();

			const VkSampler& getSampler() const { return m_Sampler; }

		private:
			VkSampler m_Sampler = VK_NULL_HANDLE;

		};


		class YzVkImage {
		public:
			YzVkImage();
			~YzVkImage();

			void createTexture(uint32_t width, uint32_t height, VkBuffer buffer);
			void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
			void createImageView(VkFormat format, VkImageAspectFlags flags);
			void createSampler();

			void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
			void cleanUp();

		public:
			static YzVkImage* createEmptyTexture(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlagBits flagBits);
			static YzVkImage* createDepthStencilBuffer(VkFormat format, uint32_t width, uint32_t height);

			static YzVkImage* createTexture2D(const std::string& filePath);

		public:
			VkImage			m_Image;
			VkDeviceMemory	m_ImageMemory;
			VkImageView		m_ImageView;
			YzVkSampler		m_YzSampler;
		};

	}
}


#endif // !YAREZO_VK_IMAGE_H
