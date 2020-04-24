#ifndef YAREZO_MODEL_H
#define YAREZO_MODEL_H

#include "Graphics/Mesh.h"

#include <string>

namespace Yarezo::Graphics {

    class Model {
    public:
        Model(const std::string& filePath);
        ~Model();

        Mesh* getMesh() const { return m_Mesh; }

    private:
        void load();

        Mesh* m_Mesh;
        std::string m_FilePath;
    };

}

#endif YAREZO_MODEL_H
