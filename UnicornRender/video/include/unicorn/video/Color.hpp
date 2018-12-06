/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_COLOR_HPP
#define UNICORN_VIDEO_COLOR_HPP

#include <glm/glm.hpp>

namespace unicorn
{
namespace video
{
/** @brief Class with some predefined colors */
struct Color
{
    static glm::vec3 Red();
    static glm::vec3 Green();
    static glm::vec3 Blue();
    static glm::vec3 White();
    static glm::vec3 Black();
    static glm::vec3 Pink();
    static glm::vec3 LightPink();
};
}
}

#endif // UNICORN_VIDEO_COLOR_HPP
