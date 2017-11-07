/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERA2DCONTROLLER_HPP
#define UNICORN_VIDEO_CAMERA2DCONTROLLER_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/CameraController.hpp>

namespace unicorn
{
namespace video
{

/** @brief 2D camera controller */
class Camera2DController : public CameraController
{
public:
    /**
     * @brief Constructs camera 2d controller
     *
     * @param cameraView target camera view matix
     */
    UNICORN_EXPORT Camera2DController(glm::mat4& cameraView);
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERA2DCONTROLLER_HPP
