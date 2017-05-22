/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/ShaderModule.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
ShaderModule::ShaderModule()
{
}

ShaderModule::~ShaderModule()
{
    Destroy();
}

vk::Result ShaderModule::Create(vk::Device device, size_t size, void* code)
{
    m_device = device;

    vk::ShaderModuleCreateInfo shaderInfo;
    shaderInfo.setCodeSize(size);
    shaderInfo.setPCode(reinterpret_cast<uint32_t*>(code));

    return m_device.createShaderModule(&shaderInfo, nullptr, &m_shaderModule);
}

void ShaderModule::Destroy()
{
    if(m_shaderModule)
    {
        m_device.destroyShaderModule(m_shaderModule, nullptr);
        m_shaderModule = nullptr;
    }
}

vk::ShaderModule& ShaderModule::GetVkShaderModule()
{
    return m_shaderModule;
}
}
}
}
