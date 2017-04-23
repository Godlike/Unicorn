/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/Shader.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            Shader::Shader()
            {
            }

            Shader::~Shader()
            {
            }

            vk::Result Shader::Create(vk::Device device, size_t size, void* code)
            {
                m_device = device;

                vk::ShaderModuleCreateInfo shaderInfo;
                shaderInfo.setCodeSize(size);
                shaderInfo.setPCode(reinterpret_cast<uint32_t*>(code));

                vk::Result result = m_device.createShaderModule(&shaderInfo, nullptr, &m_shaderModule);
                return result;
            }

            void Shader::Destroy()
            {
                m_device.destroyShaderModule(m_shaderModule, nullptr);
            }

            vk::ShaderModule& Shader::GetVkShaderModule()
            {
                return m_shaderModule;
            }
        }
    }
}