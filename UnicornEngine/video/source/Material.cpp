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
Material::Material() : m_color(Color::Red)
                     , m_isColored(true)
                     , m_isWired(false)
                     , m_isAlbedoExist(false)
                     , m_albedo(nullptr)
{
}

void Material::SetColor(glm::vec3 const& color)
{
    m_color = color;
}

void Material::SetAlbedo(Texture& albedo)
{
    m_albedo = &albedo;
    m_isColored = false;
    m_isAlbedoExist = true;
}

void Material::SetIsWired(bool wireframe)
{
    m_isWired = wireframe;
}

void Material::SetIsColored(bool colored)
{
    m_isColored = colored;
}

void Material::RemoveAlbedo()
{
    m_albedo = nullptr;
    m_isColored = true;
    m_isAlbedoExist = false;
}

bool Material::IsColored() const
{
    return m_isColored;
}

bool Material::IsWired() const
{
    return m_isWired;
}

bool Material::AlbedoExist() const
{
    return m_isAlbedoExist;
}

glm::vec3 Material::GetColor() const
{
    return m_color;
}

const Texture& Material::GetAlbedo() const
{
    return *m_albedo;
}
}
}
