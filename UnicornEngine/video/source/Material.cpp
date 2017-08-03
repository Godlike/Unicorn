/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Material.hpp>
#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace video
{
    Material::Material(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) : m_isInitialized(false)
    {
        unicorn::utility::asset::SimpleStorage& storage = unicorn::utility::asset::SimpleStorage::Instance();
        unicorn::utility::asset::Handler m_vertShaderHandler = storage.Get(vertexShaderPath);
        unicorn::utility::asset::Handler m_fragShaderHandler = storage.Get(fragmentShaderPath);

        if (!m_vertShaderHandler.IsValid() )
        {
            LOG_ERROR("Can't find shader at path : ", vertexShaderPath.c_str());
            m_isInitialized = false;
        }
        if (!m_fragShaderHandler.IsValid())
        {
            LOG_ERROR("Can't find shader at path : ", fragmentShaderPath.c_str());
            m_isInitialized = false;
        }
        m_isInitialized = true;
    }

    const std::vector<uint8_t>&  unicorn::video::Material::GetFragmentShaderContent() const
    {
        return m_fragShaderHandler.GetContent().GetBuffer();
    }

    const std::vector<uint8_t>& unicorn::video::Material::GetVertexShaderContent() const
    {
        return m_vertShaderHandler.GetContent().GetBuffer();
    }

    bool Material::IsInitialized() const
    {
        return m_isInitialized;
    }

void Material::AddUniform(uint32_t bindingLocation, const std::string& uniformName)
{
        m_uniforms.insert({bindingLocation, uniformName});
}

unicorn::utility::asset::Handler const& Material::GetVertShaderHandler() const
{
    return m_vertShaderHandler;
}

unicorn::utility::asset::Handler const& Material::GetFragShaderHandler() const
{
    return m_fragShaderHandler;
}
}
}
