/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERA_FPS_CONTROLLER_HPP
#define UNICORN_VIDEO_CAMERA_FPS_CONTROLLER_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/Camera.hpp>

namespace unicorn
{
namespace video
{
/**
 * @brief Camera controller "FPS like".
 */
class CameraFpsController
{
public:
    UNICORN_EXPORT CameraFpsController(unicorn::video::Camera& camera);
    UNICORN_EXPORT void MoveUp(float deltaTime) const;
    UNICORN_EXPORT void MoveDown(float deltaTime) const;
    UNICORN_EXPORT void MoveLeft(float deltaTime) const;
    UNICORN_EXPORT void MoveRight(float deltaTime) const;
    UNICORN_EXPORT void MoveForward(float deltaTime) const;
    UNICORN_EXPORT void MoveBackward(float deltaTime) const;
    UNICORN_EXPORT void UpdateView(double posX, double posY);
    UNICORN_EXPORT void Scroll(float yoffset);
    float sensitivity, speed;
private:
    double m_lastX, m_lastY, m_yaw, m_pitch;
    bool m_dirty;
    unicorn::video::Camera& m_camera;
};
}
}

#endif // UNICORN_VIDEO_CAMERA_FPS_CONTROLLER_HPP
