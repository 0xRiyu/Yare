#ifndef YARE_MATERIAL_H
#define YARE_MATERIAL_H

#include "Graphics/Vulkan/Image.h"

#include <string>

namespace Yare::Graphics {

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

#endif //YARE_MATERIAL_H
