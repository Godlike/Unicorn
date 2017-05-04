/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_SHADER_PROGRAM_HPP
#define UNICORN_VIDEO_SHADER_PROGRAM_HPP

#include <unicorn/video/geometry/Mesh.hpp>

#include <vulkan/vulkan.hpp>
#include <array>
#include <string>

namespace unicorn
{
namespace video
{
namespace vulkan
{
class ShaderProgram
{
public:
    ShaderProgram(vk::Device m_device, const std::string& vertShader, const std::string& fragShader);
    bool IsCreated();
    vk::PipelineShaderStageCreateInfo* GetShaderStageInfoData();
    vk::PipelineVertexInputStateCreateInfo GetVertexInputInfo();
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
    vk::ShaderModule m_vertShaderModule, m_fragShaderModule; //TODO: use vulkan::Shader here
    vk::PipelineVertexInputStateCreateInfo m_vertexInputInfo;
};
}
}
}

#endif // UNICORN_VIDEO_SHADER_PROGRAM_HPP