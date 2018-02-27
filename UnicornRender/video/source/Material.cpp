/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Material.hpp>
#include <unicorn/video/Texture.hpp>

#include <unicorn/utility/InternalLoggers.hpp>

namespace unicorn
{
namespace video
{
Material::Material()
    : m_color(Color::Red())
    , m_normSpriteArea(0, 0, 1, 1)
    , m_spriteArea(0)
    , m_isColored(true)
    , m_isWired(false)
    , m_isVisible(true)
    , m_albedo(nullptr)
{
}

void Material::SetAlbedo(std::shared_ptr<Texture> albedo)
{
    if(nullptr == albedo)
    {
        m_isColored = true;

        m_albedo = nullptr;

        return;
    }

    if(!albedo->IsLoaded())
    {
        LOG_VIDEO->Error("Setting not loaded texture to material!");

        return;
    }

    m_isColored = false;

    m_albedo = albedo;

    if(m_spriteArea == glm::vec4(0))
    {
        SetSpriteArea(0, 0, m_albedo->Width(), m_albedo->Height());
    }
    else
    {
        NormalizeSpriteArea();
    };

    DataUpdated.emit();
}

void Material::SetIsWired(bool wireframe)
{
    m_isWired = wireframe;

    DataUpdated.emit();
}

void Material::SetIsColored(bool colored)
{
    m_isColored = colored;

    DataUpdated.emit();
}

void Material::RemoveAlbedo()
{
    SetAlbedo(nullptr);
}

bool Material::IsColored() const
{
    return m_isColored;
}

bool Material::IsWired() const
{
    return m_isWired;
}

void Material::SetIsVisible(bool visible)
{
    m_isVisible = visible;

    DataUpdated.emit();
}

bool Material::IsVisible() const
{
    return m_isVisible;
}

std::shared_ptr<Texture> Material::GetAlbedo() const
{
    return m_albedo;
}

void Material::SetColor(glm::vec3 color)
{
    m_color = color;

    DataUpdated.emit();
}

glm::vec3 Material::GetColor() const
{
    return m_color;
}

void Material::SetSpriteArea(int32_t x, int32_t y, int32_t width, int32_t height)
{
    m_spriteArea = { x, y, width, height };

    NormalizeSpriteArea();

    DataUpdated.emit();
}

glm::vec4 Material::GetSpriteArea() const
{
    return m_spriteArea;
}

glm::vec4 Material::GetNormalizedSpriteArea() const
{
    return m_normSpriteArea;
}

void Material::NormalizeSpriteArea()
{
    if (m_albedo->IsLoaded())
    {
        m_normSpriteArea = {
            m_spriteArea.x / m_albedo->Width(),
            m_spriteArea.y / m_albedo->Height(),
            m_spriteArea.z / m_albedo->Width(),
            m_spriteArea.w / m_albedo->Height() };
    }
}

}
}
