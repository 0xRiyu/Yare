#include "Graphics/Material.h"
#include "Core/Yzh.h"

namespace Yarezo::Graphics {

    Material::Material(const std::string& textureFilePath, MaterialTexType type)
        : m_FilePaths({textureFilePath}), m_Type(type) {
    }

    Material::Material(const std::vector<std::string>& textureFilePaths, MaterialTexType type)
        : m_FilePaths(textureFilePaths), m_Type(type) {
    }

    Material::~Material() {
        if (m_Texture) {
            delete m_Texture;
        }
    }

    void Material::loadTextures() {
        switch (m_Type) {
        case MaterialTexType::TextureCube: {
            std::vector<std::string> texturePaths{6};
            int index = 0;
            for (int i = 0; i < 6; i++){
                if (index < m_FilePaths.size()) {
                    texturePaths[i] = m_FilePaths[index];
                    index++;
                } else {
                    texturePaths[i] = "../YareZo/Resources/Textures/default.jpg";
                }
            }

            m_Texture = YzVkImage::createTextureCube(texturePaths);
            break;
        }
        case MaterialTexType::Texture2D: {
            if (m_FilePaths.size() >= 1) {
                m_Texture = YzVkImage::createTexture2D(m_FilePaths[0]);
            } else {
                m_Texture = YzVkImage::createTexture2D("../YareZo/Resources/Textures/default.jpg");
            }
            break;
        }
        }
    }

}
