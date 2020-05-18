#include "Graphics/Material.h"

namespace Yarezo::Graphics {

    Material::Material(const std::string& textureFilePath, MaterialTexType type) {
        if (!textureFilePath.empty()) {
            loadTexture(textureFilePath, type);
        } else {
            loadTexture("../YareZo/Resources/Textures/default.jpg", type);
        }
    }

    Material::~Material() {
        if (m_Texture) {
            delete m_Texture;
        }
    }

    void Material::loadTexture(const std::string& textureFilePath, MaterialTexType type) {
        switch (type) {
        case MaterialTexType::TextureCube: {
            m_Texture = YzVkImage::createTextureCube(textureFilePath);
            break;
        }
        case MaterialTexType::Texture2D: {
            m_Texture = YzVkImage::createTexture2D(textureFilePath);
            break;
        }
        }
    }

}
