/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/ShaderProgram.hpp>
#include <unicorn/utility/asset/SimpleStorage.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/geometry/Mesh.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
ShaderProgram::ShaderProgram(vk::Device device, const std::string& vertShader, const std::string& fragShader)
    : m_isCreated(false), m_device(device)
{
    unicorn::utility::asset::SimpleStorage& storage = unicorn::utility::asset::SimpleStorage::Instance();
    unicorn::utility::asset::Handler simpleVertShaderHandler = storage.Get("data/shaders/shader.vert.spv");
    unicorn::utility::asset::Handler simpleFragShaderHandler = storage.Get("data/shaders/shader.frag.spv");

    if (!simpleVertShaderHandler.IsValid() || !simpleFragShaderHandler.IsValid())
    {
        LOG_ERROR("Can't find shaders!");
        m_isCreated = false;
    }

    bool shadersCreatedFailed = !CreateShaderModule(simpleVertShaderHandler.GetContent().GetBuffer(), m_vertShaderModule) ||
                                !CreateShaderModule(simpleFragShaderHandler.GetContent().GetBuffer(), m_fragShaderModule);

    if (shadersCreatedFailed)
    {
        LOG_ERROR("Can't create shader module!");
        m_isCreated = false;
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
    m_bindingDescription.setStride(sizeof(geometry::Vertex));
    m_bindingDescription.setInputRate(vk::VertexInputRate::eVertex);
}

void ShaderProgram::CreateAttributeDescription()
{
    //Position
    m_attributeDescription.at(0).setBinding(0);
    m_attributeDescription.at(0).setLocation(0);
    m_attributeDescription.at(0).setFormat(vk::Format::eR32G32B32Sfloat);
    m_attributeDescription.at(0).setOffset(offsetof(geometry::Vertex, pos));
    //Color
    m_attributeDescription.at(1).setBinding(0);
    m_attributeDescription.at(1).setLocation(1);
    m_attributeDescription.at(1).setFormat(vk::Format::eR32G32B32Sfloat);
    m_attributeDescription.at(1).setOffset(offsetof(geometry::Vertex, color));
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
