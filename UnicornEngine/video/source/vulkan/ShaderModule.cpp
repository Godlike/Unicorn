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
            }

            vk::Result ShaderModule::Create(vk::Device device, size_t size, void* code)
            {
                m_device = device;

                vk::ShaderModuleCreateInfo shaderInfo;
                shaderInfo.setCodeSize(size);
                shaderInfo.setPCode(reinterpret_cast<uint32_t*>(code));

                vk::Result result = m_device.createShaderModule(&shaderInfo, nullptr, &m_shaderModule);
                return result;
            }

            void ShaderModule::Destroy()
            {
                m_device.destroyShaderModule(m_shaderModule, nullptr);
            }

            vk::ShaderModule& ShaderModule::GetVkShaderModule()
            {
                return m_shaderModule;
            }
        }
    }
}