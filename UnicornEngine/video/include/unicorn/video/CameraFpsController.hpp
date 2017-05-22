/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERA_FPS_CONTROLLER_HPP
#define UNICORN_VIDEO_CAMERA_FPS_CONTROLLER_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/CameraController.hpp>

namespace unicorn
{
namespace video
{
/**
 * @brief Simple template for quick prototyping game - FPS controller.
 */
class CameraFpsController : public CameraController
{
public:
    UNICORN_EXPORT CameraFpsController(std::shared_ptr<Camera> camera);
    UNICORN_EXPORT void MoveUp(float deltaTime);
    UNICORN_EXPORT void MoveDown(float deltaTime);
    UNICORN_EXPORT void MoveLeft(float deltaTime);
    UNICORN_EXPORT void MoveRight(float deltaTime);
    UNICORN_EXPORT void MoveForward(float deltaTime);
    UNICORN_EXPORT void MoveBackward(float deltaTime);
    UNICORN_EXPORT void UpdateMouseView(double posX, double posY);
    UNICORN_EXPORT void Scroll(double yoffset);
    UNICORN_EXPORT void SetCenterPosition(double posX, double posY);
    float sensitivity, speed;
private:
    double m_lastX, m_lastY, m_yaw, m_pitch;
    bool m_firstMouse;
};
}
}

#endif // UNICORN_VIDEO_CAMERA_FPS_CONTROLLER_HPP
