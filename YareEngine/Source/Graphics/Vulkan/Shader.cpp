#include "Graphics/Vulkan/Shader.h"

#include <map>

#include "Graphics/Vulkan/Devices.h"
#include "Graphics/Vulkan/Utilities.h"
#include "Utilities/IOHelper.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {

    bool begins_with(const std::string& input, const std::string& match) {
        return input.size() >= match.size() && equal(match.begin(), match.end(), input.begin());
    }

    Shader::Shader(const std::string& filePath, const std::string& shaderName)
        : m_ShaderStages(nullptr), m_ShaderName(shaderName), m_FilePath(filePath) {
        m_ShaderSource = Utilities::readFile(filePath + "/" + shaderName);
        readShaderFiles();
    }

    Shader::~Shader() {
        unloadModules();
        delete[] m_ShaderStages;
    }

    void Shader::unloadModules() {
        for (uint8_t i = 0; i < m_StageCount; i++) {
            vkDestroyShaderModule(Devices::instance()->getDevice(), m_ShaderStages[i].module, nullptr);
        }
    }

    void Shader::readShaderFiles() {
        m_StageCount = 0;
        uint32_t currentShaderStage = 0;

        std::map<ShaderType, std::string> shaderFiles;

        for (auto iter = m_ShaderSource.begin(); iter < m_ShaderSource.end(); iter++) {
            std::string current_line = *iter;
            if (begins_with(current_line, "//SHADER:")) {
                if (current_line.find("VERTEX") != std::string::npos) {
                    shaderFiles.insert(std::pair<ShaderType, std::string>(ShaderType::Vertex, *(std::next(iter))));
                    m_StageCount++;
                    continue;
                } else if (current_line.find("FRAGMENT") != std::string::npos) {
                    shaderFiles.insert(std::pair<ShaderType, std::string>(ShaderType::Fragment, *(std::next(iter))));
                    m_StageCount++;
                    continue;
                } else if (current_line.find("GEOMETRY") != std::string::npos) {
                    shaderFiles.insert(std::pair<ShaderType, std::string>(ShaderType::Geometry, *(std::next(iter))));
                    m_StageCount++;
                    continue;
                } else if (current_line.find("COMPUTE") != std::string::npos) {
                    shaderFiles.insert(std::pair<ShaderType, std::string>(ShaderType::Compute, *(std::next(iter))));
                    m_StageCount++;
                    continue;
                } else if (current_line.find("TESSELLATION CONTROL") != std::string::npos) {
                    shaderFiles.insert(
                        std::pair<ShaderType, std::string>(ShaderType::Tessellation_Control, *(std::next(iter))));
                    m_StageCount++;
                    continue;
                } else if (current_line.find("TESSELLATION EVALUATION") != std::string::npos) {
                    shaderFiles.insert(
                        std::pair<ShaderType, std::string>(ShaderType::Tessellation_Evaluation, *(std::next(iter))));
                    m_StageCount++;
                    continue;
                } else {
                    YZ_CRITICAL("Line " + std::to_string(iter - m_ShaderSource.begin() + 1) + " in file '" +
                                m_ShaderName + "'  has //SHADER:, but contained an unknown shader type");
                }
            }
        }

        m_ShaderStages = new VkPipelineShaderStageCreateInfo[m_StageCount]();

        for (auto file : shaderFiles) {
            m_ShaderType.push_back(file.first);
            auto rawShader = VkUtil::readShaderFile(m_FilePath + "//" + file.second);

            VkShaderModuleCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            createInfo.codeSize = rawShader.size();
            createInfo.pCode = reinterpret_cast<const uint32_t*>(rawShader.data());

            m_ShaderStages[currentShaderStage].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            m_ShaderStages[currentShaderStage].stage = static_cast<VkShaderStageFlagBits>(file.first);
            m_ShaderStages[currentShaderStage].pName = "main";

            auto res = vkCreateShaderModule(Devices::instance()->getDevice(), &createInfo, nullptr,
                                            &m_ShaderStages[currentShaderStage].module);
            if (res != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan was unable to create a shaderModule with provided shader code.");
            }

            currentShaderStage++;
        }
    }
}  // namespace Yare::Graphics
