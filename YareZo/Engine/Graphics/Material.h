#ifndef YAREZO_MATERIAL_H
#define YAREZO_MATERIAL_H

#include "Platform/Vulkan/Vk_Image.h"

#include <string>

namespace Yarezo::Graphics {

    enum class MaterialTexType { TextureCube, Texture2D };

    class Material {
    public:
        Material(const std::vector<std::string>& textureFilePaths = {}, MaterialTexType type = MaterialTexType::Texture2D);
        virtual ~Material();

        void loadTexture(const std::vector<std::string>& textureFilePaths, MaterialTexType type);
        const YzVkImage* getTextureImage() const { return m_Texture; }

    private:
        YzVkImage* m_Texture;
    };

}

#endif //YAREZO_MATERIAL_H
