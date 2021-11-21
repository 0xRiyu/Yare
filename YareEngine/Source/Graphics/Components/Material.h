#ifndef YARE_MATERIAL_H
#define YARE_MATERIAL_H

#include <string>

#include "Component.h"
#include "Graphics/Vulkan/Image.h"

namespace Yare::Graphics {

    enum class MaterialTexType { TextureCube, Texture2D };

    class Material : public Component {
       public:
        Material(const std::string& textureFilePath, MaterialTexType type = MaterialTexType::Texture2D);
        Material(const std::vector<std::string>& textureFilePaths = {},
                 MaterialTexType                 type = MaterialTexType::Texture2D);

        virtual ~Material();

        void loadTextures();
        void setImageIdx(int idx) { m_ImageIdx = idx; }

        const Image* getTextureImage() const { return m_Texture; }
        int          getImageIdx() const { return m_ImageIdx; }

       private:
        Image*                   m_Texture;
        MaterialTexType          m_Type;
        int                      m_ImageIdx = 0;
        std::vector<std::string> m_FilePaths;
    };

}  // namespace Yare::Graphics

#endif  // YARE_MATERIAL_H
