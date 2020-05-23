#include "Graphics/Material.h"
#include "Core/Yzh.h"

namespace Yarezo::Graphics {

    Material::Material(const std::vector<std::string>& textureFilePaths, MaterialTexType type) {
        if (!textureFilePaths.empty()) {
            loadTexture(textureFilePaths, type);
        } else {
            loadTexture({"../YareZo/Resources/Textures/default.jpg"}, type);
        }
    }

    Material::~Material() {
        if (m_Texture) {
            delete m_Texture;
        }
    }

    void Material::loadTexture(const std::vector<std::string>& textureFilePaths, MaterialTexType type) {
        if (textureFilePaths.empty()) {
            YZ_CRITICAL("No texture files were provided to the Material");
        }

        switch (type) {
        case MaterialTexType::TextureCube: {
            m_Texture = YzVkImage::createTextureCube(textureFilePaths);
            break;
        }
        case MaterialTexType::Texture2D: {
            m_Texture = YzVkImage::createTexture2D(textureFilePaths[0]);
            break;
        }
        }
    }

}
