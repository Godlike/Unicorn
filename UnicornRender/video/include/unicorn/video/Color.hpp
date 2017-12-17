/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_COLOR_HPP
#define UNICORN_VIDEO_COLOR_HPP

#include <unicorn/utility/SharedMacros.hpp>

#include <glm/glm.hpp>

namespace unicorn
{
namespace video
{
/** @brief Class with some predefined colors */
struct Color
{
    UNICORN_EXPORT static glm::vec3 Red();
    UNICORN_EXPORT static glm::vec3 Green();
    UNICORN_EXPORT static glm::vec3 Blue();
    UNICORN_EXPORT static glm::vec3 White();
    UNICORN_EXPORT static glm::vec3 Black();
    UNICORN_EXPORT static glm::vec3 Pink();
    UNICORN_EXPORT static glm::vec3 LightPink();
};
}
}

#endif // UNICORN_VIDEO_COLOR_HPP
