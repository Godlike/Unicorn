#include <vorpal/graphics/Texture.hpp>
#include <vorpal/utility/asset/SimpleStorage.hpp>
#include <vorpal/utility/Logger.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool vp::graphics::Texture::LoadFromFile(const std::string& path)
{
    vp::utility::asset::SimpleStorage& storage = vp::utility::asset::SimpleStorage::Instance();
    vp::utility::asset::Handler textureHandler = storage.Get(path.c_str());
    if (!textureHandler.IsValid())
    {
        LOG_ERROR("Failed to load texture! Path: %s", path.c_str());
        return false;
    }
    const std::vector<uint8_t>& textureContent = textureHandler.GetContent().GetBuffer();
    stbi_uc* pixels = nullptr;
    pixels = stbi_load_from_memory(textureContent.data(), static_cast<int>(textureContent.size()), &m_width, &m_height, &m_channels, STBI_rgb_alpha); //TODO: check m_channels variable, remove 4 as hardcode!
    //VkDeviceSize imageSize = m_width * m_height * 4;
    if (!pixels)
    {
        LOG_ERROR("Found texture, but can't load it!");
        return false;
    }
    LOG_INFO("Texture: %s , succesfully loaded!", path.c_str());
    return true;
}
