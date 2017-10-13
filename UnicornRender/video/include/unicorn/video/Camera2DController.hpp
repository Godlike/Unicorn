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
    UNICORN_EXPORT Camera2DController(glm::mat4& cameraView);

    UNICORN_EXPORT void MoveUp(float distance);
    UNICORN_EXPORT void MoveDown(float distance);
    UNICORN_EXPORT void MoveLeft(float distance);
    UNICORN_EXPORT void MoveRight(float distance);
private:
    virtual void UpdateViewMatrix();
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERA2DCONTROLLER_HPP
