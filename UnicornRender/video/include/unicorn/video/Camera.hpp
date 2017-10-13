/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERA_HPP
#define UNICORN_VIDEO_CAMERA_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/system/Window.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <wink/slot.hpp>

#include <utility>

namespace unicorn
{
namespace video
{

/** @brief Camera struct, holds view and projection matrices. */
struct Camera
{
    glm::mat4 projection = glm::mat4();
    glm::mat4 view = glm::mat4();
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERA_HPP
