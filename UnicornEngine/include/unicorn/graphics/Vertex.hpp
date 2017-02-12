/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_GRAPHICS_VERTEX_HPP
#define UNICORN_GRAPHICS_VERTEX_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>

namespace uc
{
namespace graphics
{
class Vertex
{
public:    
    glm::vec3 pos;
    glm::vec2 texCoord;
};
}
}

#endif // UNICORN_GRAPHICS_VERTEX_HPP
