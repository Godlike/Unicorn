/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_SHADER_PROGRAM_HPP
#define UNICORN_VIDEO_SHADER_PROGRAM_HPP

#include <vulkan/vulkan.hpp>
#include <array>

namespace unicorn
{
namespace video
{
namespace vulkan
{
/**
* @brief Abstraction for shader program, which renderer uses for rendering meshes
*/
class ShaderProgram
{
public:
    /**
    * @brief Constructor
    * @param device device to allocate from
    * @param vertShaderPath pth to vertex shader path
    * @param fragShaderPath path to fragment shader
    * @attention Always use 'main' as entry point
    */
    ShaderProgram(vk::Device device, const std::string& vertShaderPath, const std::string& fragShaderPath);
    /**
    * @brief Function to check is shader was successfully loaded and created
    * @return true if shader was created and false if not
    */
    bool IsCreated();
    /**
    * @brief Getter for shader stage info data
    * @return pointer to all shader stage info data
    */
    vk::PipelineShaderStageCreateInfo* GetShaderStageInfoData();
    /**
    * @brief Getter for vertex input info
    * @return pipeline vertex input state create info
    */
    vk::PipelineVertexInputStateCreateInfo GetVertexInputInfo();
    /**
    * @brief Destroys shader modules
    */
    void DestroyShaderModules();
private:
    bool m_isCreated;
    void CreateBindingDescription();
    void CreateAttributeDescription();
    bool CreateShaderModule(const std::vector<uint8_t>& code, vk::ShaderModule& shaderModule);
    void CreateVertexInputInfo();

    vk::Device m_device;
    vk::VertexInputBindingDescription m_bindingDescription;
    std::array<vk::VertexInputAttributeDescription, 2> m_attributeDescription;
    std::array<vk::PipelineShaderStageCreateInfo, 2> m_shaderStages;
    vk::ShaderModule m_vertShaderModule, m_fragShaderModule;
    vk::PipelineVertexInputStateCreateInfo m_vertexInputInfo;
};
}
}
}

#endif // UNICORN_VIDEO_SHADER_PROGRAM_HPP
