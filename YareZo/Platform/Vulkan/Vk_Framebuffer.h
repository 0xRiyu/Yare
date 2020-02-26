#ifndef YAREZO_VK_FRAMEBUFFER_H
#define YAREZO_VK_FRAMEBUFFER_H

#include "Platform/Vulkan/Vk.h"

namespace Yarezo {
    namespace Graphics {

        struct FramebufferInfo {
            VkStructureType type;
            uint32_t width;
            uint32_t height;
            uint32_t layers = 1;
            uint32_t attachmentCount = 1;
            VkImageView attachments;
        };

        class YzVkFramebuffer {


        public:
            YzVkFramebuffer(const FramebufferInfo& framebufferInfo);
            ~YzVkFramebuffer();

            inline const VkFramebuffer& getFramebuffer() const { return m_Framebuffer; }

        private:

            VkFramebuffer m_Framebuffer;

        };


    }
}



#endif