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
class Material
{
public:
    UNICORN_EXPORT Material(bool isColored);
private:
    glm::vec3 m_color = Color::Red; // Always default red for everything to detect visual errors
    bool m_isColored = false;
};
}
}

#endif // UNICORN_VIDEO_MATERIAL_HPP
