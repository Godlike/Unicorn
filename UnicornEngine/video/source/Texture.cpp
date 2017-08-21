/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/Texture.hpp>
#include <unicorn/utility/asset/SimpleStorage.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace unicorn
{
namespace video
{
    Texture::Texture() : m_width(0)
        , m_height(0)
        , m_channels(0)
        , m_size(0)
        , m_data(nullptr)
        , m_path(nullptr)
        , m_initialized(false)
    {

    }

Texture::Texture(const std::string& path) : Texture()
{
    Load(path);
}

void Texture::Delete()
{
    if(m_data)
    {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
    m_initialized = false;
}

int32_t Texture::Size() const
{
    return m_size;
}

unsigned char* Texture::Data() const
{
    return m_data;
}

int32_t Texture::Width() const
{
    return m_width;
}

int32_t Texture::Height() const
{
    return m_height;
}

const char* Texture::Path() const
{
    return m_path;
}

bool Texture::Load(const std::string& path)
{    
     if(!m_initialized)
     {
         m_path = path.c_str();

         unicorn::utility::asset::SimpleStorage& storage = unicorn::utility::asset::SimpleStorage::Instance();
         unicorn::utility::asset::Handler textureHandler = storage.Get(path.c_str());

         m_data = stbi_load_from_memory(textureHandler.GetContent().GetBuffer().data(),
             textureHandler.GetContent().GetBuffer().size(),
             &m_width, &m_height, &m_channels, STBI_rgb_alpha);
         m_size = m_width * m_height * 4;

         if (!m_data) {
             LOG_ERROR("Failed to load texture image with path - ", m_path);
             return false;
         }
         std::hash<std::string> hash_fn;
         m_id = hash_fn(path);
         m_initialized = true;
     }
    return true;
}

bool Texture::IsLoaded() const
{
    return m_initialized;
}
}
}
