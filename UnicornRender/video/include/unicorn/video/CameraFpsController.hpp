/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERAFPSCONTROLLER_HPP
#define UNICORN_VIDEO_CAMERAFPSCONTROLLER_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/CameraController.hpp>

namespace unicorn
{
namespace video
{

/** @brief FPS style camera controller */
class CameraFpsController : public CameraController
{
public:
    UNICORN_EXPORT CameraFpsController(glm::mat4& cameraView);
    UNICORN_EXPORT void Roll(float degrees);
    UNICORN_EXPORT void MoveUp(float distance);
    UNICORN_EXPORT void MoveDown(float distance);
    UNICORN_EXPORT void MoveLeft(float distance);
    UNICORN_EXPORT void MoveRight(float distance);
    UNICORN_EXPORT void MoveForward(float distance);
    UNICORN_EXPORT void MoveBackward(float distance);

    /** @brief Updates view matrix by taking diff between past and new mouse coordinates */
    UNICORN_EXPORT void UpdateView(double x, double y);

    /** @brief Sets mouse coordinates without updating view matrix */
    UNICORN_EXPORT void SetViewPositions(double x, double y);

    //! Mouse sensitivity for x axis
    float sensitivityX;
    //! Mouse sensitivity for y axis
    float sensitivityY;
protected:
    virtual void UpdateViewMatrix();
private:
    glm::quat m_quat;
    glm::vec2 m_mousePosition;
    double m_yaw;
    double m_pitch;
    double m_roll;
    bool m_dirtyViewPosition;
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERAFPSCONTROLLER_HPP
