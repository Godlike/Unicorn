/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Color.hpp>

namespace unicorn
{
    namespace video
    {
        glm::vec3 Color::Red = { 1.0f, 0.0f, 0.0f } ;
        glm::vec3 Color::Green =  { 0.0f, 1.0f, 0.0f } ;
        glm::vec3 Color::Blue = { 0.0f, 0.0f, 1.0f };
        glm::vec3 Color::White = { 1.0f, 1.0f, 1.0f };
        glm::vec3 Color::Black = { 0.0f, 0.0f, 0.0f };
    }
}