/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_COLOR_HPP
#define UNICORN_VIDEO_COLOR_HPP

#include <glm/glm.hpp>
#include <unicorn/utility/SharedMacros.hpp>

namespace unicorn
{
    namespace video
    {
        struct Color
        {
            UNICORN_EXPORT static glm::vec3 Red;
            UNICORN_EXPORT static glm::vec3 Green;
            UNICORN_EXPORT static glm::vec3 Blue;
            UNICORN_EXPORT static glm::vec3 White;
            UNICORN_EXPORT static glm::vec3 Black;
        };
    }
}

#endif // UNICORN_VIDEO_COLOR_HPP