/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Material.hpp>
#include <unicorn/video/Texture.hpp>
#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace video
{
Material::Material() : color(Color::Red())
                     , m_isColored(true)
                     , m_isWired(false)
                     , m_albedo(nullptr)
{
}

void Material::SetAlbedo(Texture const* albedo)
{
    m_isColored = false;
    if(albedo == nullptr || !albedo->IsLoaded())
    {
        LOG_ERROR("Setting not loaded texture to material!");
        return;
    }
    m_albedo = albedo;
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
    SetAlbedo(nullptr);
    m_isColored = true;
}

bool Material::IsColored() const
{
    return m_isColored;
}

bool Material::IsWired() const
{
    return m_isWired;
}

const Texture* Material::GetAlbedo() const
{
    return m_albedo;
}
}
}
