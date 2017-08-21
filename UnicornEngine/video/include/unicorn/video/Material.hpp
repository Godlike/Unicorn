/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MATERIAL_HPP
#define UNICORN_VIDEO_MATERIAL_HPP

#include <unicorn/video/Color.hpp>

#include <glm/glm.hpp>

namespace unicorn
{
namespace video
{
class Texture;

class Material
{
public:
    UNICORN_EXPORT Material();
    UNICORN_EXPORT void SetColor(const glm::vec3 color);
    UNICORN_EXPORT void SetAlbedo(Texture& texture);
    UNICORN_EXPORT void RemoveAlbedo();
    UNICORN_EXPORT bool IsColored() const;
    UNICORN_EXPORT glm::vec3 GetColor() const
    {
        return m_color;
    }
private:
    glm::vec3 m_color;
    bool m_isColored;
    Texture* m_albedo;
};
}
}

#endif // UNICORN_VIDEO_MATERIAL_HPP
