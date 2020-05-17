#include "Graphics/Material.h"

namespace Yarezo::Graphics {

    Material::Material(const std::string& textureFilePath) {
        if (!textureFilePath.empty()) {
            loadTexture(textureFilePath);
        } else {
            loadTexture("../YareZo/Resources/Textures/default.jpg");
        }
    }

    Material::~Material() {
        if (m_Texture) {
            m_Texture->cleanUp();
            delete m_Texture;
        }
    }

    void Material::loadTexture(const std::string& textureFilePath) {
        m_Texture = YzVkImage::createTexture2D(textureFilePath);
    }

}
