#ifndef YAREZO_MATERIAL_H
#define YAREZO_MATERIAL_H

#include "Graphics/Vulkan/Vk_Image.h"

#include <string>

namespace Yarezo::Graphics {

    enum class MaterialTexType { TextureCube, Texture2D };

    class Material {
    public:
        Material(const std::string& textureFilePath, MaterialTexType type = MaterialTexType::Texture2D);
        Material(const std::vector<std::string>& textureFilePaths = {},
                 MaterialTexType type = MaterialTexType::Texture2D);

        virtual ~Material();

        void loadTextures();
        void setImageIdx(int idx) { m_ImageIdx = idx; }

        const YzVkImage* getTextureImage() const { return m_Texture; }
        int              getImageIdx()     const { return m_ImageIdx; }

    private:
        YzVkImage* m_Texture;
        MaterialTexType m_Type;
        int m_ImageIdx = 0;
        std::vector<std::string> m_FilePaths;
    };

}

#endif //YAREZO_MATERIAL_H
