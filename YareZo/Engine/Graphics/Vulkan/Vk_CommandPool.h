#ifndef YAREZO_VK_COMMANDPOOL_H
#define YAREZO_VK_COMMANDPOOL_H

#include "Graphics/Vulkan/Vk.h"

namespace Yarezo {
    namespace Graphics {

        class YzVkCommandPool {
        public:

            YzVkCommandPool();
            ~YzVkCommandPool();

            void init();
            void cleanUp();
            const VkCommandPool& getCommandPool() const { return m_CommandPool; }

        private:

            VkCommandPool m_CommandPool;



        };


    }
}


#endif //YAREZO_VK_COMMANDPOOL_H
