#ifndef YAREZO_MATERIAL_H
#define YAREZO_MATERIAL_H

#include "Platform/Vulkan/Vk_Image.h"

#include <string>

namespace Yarezo::Graphics {

    class Material {
    public:
        Material();
        virtual ~Material();

        void loadTexture(const std::string& textureFilePath);
        const YzVkImage* getTextureImage() const { return m_Texture; }

    private:
        YzVkImage* m_Texture;
    };

}

#endif //YAREZO_MATERIAL_H
