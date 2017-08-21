/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Material.hpp>

namespace unicorn
{
namespace video
{
    Material::Material() : m_isColored(true), m_albedo(nullptr), m_color(Color::Red)
    {
       
    }

    void Material::SetColor(const glm::vec3 color)
    {
        m_color = color;
    }

    void Material::SetAlbedo(Texture& albedo)
    {
        m_albedo = &albedo;
        m_isColored = false;
    }

    void Material::RemoveAlbedo()
    {
        m_albedo = nullptr;
        m_isColored = true;
    }

    bool Material::IsColored() const
    {
        return m_isColored;
    }
}
}
