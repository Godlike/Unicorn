/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/ShaderProgram.hpp>
#include <unicorn/utility/asset/SimpleStorage.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/Mesh.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
ShaderProgram::ShaderProgram(vk::Device device, const unicorn::video::Material& material)
    : m_isCreated(false), m_device(device)
{
    bool shadersCreatedFailed = !CreateShaderModule(material.GetVertShaderHandler().GetContent().GetBuffer(), m_vertShaderModule) ||
                                !CreateShaderModule(material.GetFragShaderHandler().GetContent().GetBuffer(), m_fragShaderModule);

    if (shadersCreatedFailed)
    {
        LOG_ERROR("Can't create shader module!");
        m_isCreated = false;
        return;
    }

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
    vertShaderStageInfo.module = m_vertShaderModule;
    vertShaderStageInfo.pName = "main";

    vk::PipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
    fragShaderStageInfo.module = m_fragShaderModule;
    fragShaderStageInfo.pName = "main";

    m_shaderStages = {{vertShaderStageInfo, fragShaderStageInfo}};

    CreateBindingDescription();
    CreateAttributeDescription();
    CreateVertexInputInfo();

    m_isCreated = true;
}

void ShaderProgram::CreateBindingDescription()
{
    m_bindingDescription.setBinding(0);
    m_bindingDescription.setStride(sizeof(Vertex));
    m_bindingDescription.setInputRate(vk::VertexInputRate::eVertex);
}

void ShaderProgram::CreateAttributeDescription()
{
    //Position
    m_attributeDescription.at(0).setBinding(0);
    m_attributeDescription.at(0).setLocation(0);
    m_attributeDescription.at(0).setFormat(vk::Format::eR32G32B32Sfloat);
    m_attributeDescription.at(0).setOffset(offsetof(Vertex, pos));
    ////Color
    //m_attributeDescription.at(1).setBinding(0);
    //m_attributeDescription.at(1).setLocation(1);
    //m_attributeDescription.at(1).setFormat(vk::Format::eR32G32B32Sfloat);
    //m_attributeDescription.at(1).setOffset(offsetof(Vertex, color));
}

void ShaderProgram::CreateVertexInputInfo()
{
    m_vertexInputInfo.vertexBindingDescriptionCount = 1;
    m_vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_attributeDescription.size());
    m_vertexInputInfo.pVertexBindingDescriptions = &m_bindingDescription;
    m_vertexInputInfo.pVertexAttributeDescriptions = m_attributeDescription.data();
}

vk::PipelineShaderStageCreateInfo* ShaderProgram::GetShaderStageInfoData()
{
    return m_shaderStages.data();
}

vk::PipelineVertexInputStateCreateInfo ShaderProgram::GetVertexInputInfo()
{
    return m_vertexInputInfo;
}

bool ShaderProgram::IsCreated()
{
    return m_isCreated;
}

bool ShaderProgram::CreateShaderModule(const std::vector<uint8_t>& code, vk::ShaderModule& shaderModule)
{
    vk::Result result;
    vk::ShaderModuleCreateInfo createInfo;
    if (code.size() % sizeof(uint32_t) != 0)
    {
        LOG_ERROR("Shader code size is not multiple of sizeof(uint32_t), look at VkShaderModuleCreateInfo(3) Manual Page.");
        return false;
    }
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    result = m_device.createShaderModule(&createInfo, {}, &shaderModule);

    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create shader module!");
        return false;
    }

    return true;
}

void ShaderProgram::DestroyShaderModules()
{
    if (m_vertShaderModule)
    {
        m_device.destroyShaderModule(m_vertShaderModule);
        m_vertShaderModule = nullptr;
    }
    if (m_fragShaderModule)
    {
        m_device.destroyShaderModule(m_fragShaderModule);
        m_fragShaderModule = nullptr;
    }
}
}
}
}
