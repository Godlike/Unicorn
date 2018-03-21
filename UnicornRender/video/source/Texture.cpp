/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Texture.hpp>

#include <unicorn/utility/InternalLoggers.hpp>

#include <mule/asset/SimpleStorage.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace unicorn
{
namespace video
{
Texture::Texture(const std::string& path)
    : m_width(0)
    , m_height(0)
    , m_channels(0)
    , m_size(0)
    , m_id(0)
    , m_data(nullptr)
    , m_path(path)
    , m_initialized(false)
{
    if(!m_path.empty())
    {
        Load(path);
    }
}

Texture::~Texture()
{
    FreeData();
}

bool Texture::Load(const std::string& path)
{
    FreeData();

    m_path = path;

    mule::asset::SimpleStorage& storage = mule::asset::SimpleStorage::Instance();
    mule::asset::Handler textureHandler = storage.Get(m_path);

    if (!textureHandler.IsValid())
    {
        LOG_VIDEO->Error("Can't find texture - {}", m_path.c_str());
        return false;
    }

    m_data = stbi_load_from_memory(textureHandler.GetContent().GetBuffer().data(),
        static_cast<int>(textureHandler.GetContent().GetBuffer().size()),
        reinterpret_cast<int32_t*>(&m_width),
        reinterpret_cast<int32_t*>(&m_height),
        reinterpret_cast<int32_t*>(&m_channels),
        STBI_rgb_alpha);

    m_size = m_width * m_height * 4;

    if (!m_data)
    {
        LOG_VIDEO->Error("Failed to load texture image with path - {}", m_path.c_str());
        return false;
    }

    m_id = static_cast<uint32_t>(std::hash<std::string>{}(path));
    m_initialized = true;

    return true;
}

bool Texture::Load(unsigned char* memory, uint32_t width, uint32_t height)
{
    // TODO: memcpy memory to internal data
    m_data = memory;

    m_width = width;
    m_height = height;

    m_size = m_width * m_height;

    m_initialized = true;
    return true;
}

bool Texture::IsLoaded() const
{
    return m_initialized;
}

void Texture::FreeData()
{
    if(m_initialized && m_data)
    {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
    m_initialized = false;
}

uint32_t Texture::Size() const
{
    if(!m_initialized)
    {
        LOG_VIDEO->Warning("Texture not loaded!");
    }
    return m_size;
}

uint8_t* Texture::Data() const
{
    if(!m_initialized)
    {
        LOG_VIDEO->Warning("Texture not loaded!");
    }
    return m_data;
}

uint32_t Texture::Width() const
{
    if(!m_initialized)
    {
        LOG_VIDEO->Warning("Texture not loaded!");
    }
    return m_width;
}

uint32_t Texture::Height() const
{
    if(!m_initialized)
    {
        LOG_VIDEO->Warning("Texture not loaded!");
    }
    return m_height;
}

std::string const& Texture::Path() const
{
    if(!m_initialized)
    {
        LOG_VIDEO->Warning("Texture not loaded!");
    }
    return m_path;
}

uint32_t Texture::GetId() const
{
    if(!m_initialized)
    {
        LOG_VIDEO->Warning("Texture not loaded!");
    }
    return m_id;
}
}
}
