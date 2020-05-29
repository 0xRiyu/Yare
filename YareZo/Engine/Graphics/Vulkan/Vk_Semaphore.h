#ifndef YAREZO_VK_SEMAPHORE_H
#define YAREZO_VK_SEMAPHORE_H

#include "Graphics/Vulkan/Vk.h"

namespace Yarezo {
    namespace Graphics {

        class YzVkSemaphore {
        public:
            YzVkSemaphore();
            ~YzVkSemaphore();

            void init();
            void cleanUp();

            const VkSemaphore& getSemaphore() const { return m_Semaphore; }

        private:
            VkSemaphore m_Semaphore;
        };
    }
}

#endif // !YAREZO_VK_SEMAPHORE_H
