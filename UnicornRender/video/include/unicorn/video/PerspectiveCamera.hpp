/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_PERSPECTIVECAMERA_HPP
#define UNICORN_VIDEO_PERSPECTIVECAMERA_HPP

#include <unicorn/video/CameraProjection.hpp>
#include <unicorn/system/Window.hpp>

namespace unicorn
{
namespace video
{

static float constexpr NEAR_PERSPECTIVE_FRUSTRUM_PLANE = 0.1f;
static float constexpr FAR_PERSPECTIVE_FRUSTRUM_PLANE = 100000.f;

/** @brief Perspective camera projection controller */
class PerspectiveCamera final : public CameraProjection
{
public:
    PerspectiveCamera(system::Window& window, glm::mat4& cameraProj);

    /** @brief Sets field of view */
    void SetFov(float fov);

    /** @brief Sets near Z plane */
    void SetZNear(float znear);

    /** @brief Sets far Z plane */
    void SetZFar(float zfar);

    /** @brief Returns field of view */
    float GetFov() const;

    /** @brief Recalculates perspective */
    void UpdateProjection() override;
private:
    float m_fov;
    float m_fovLowerBound;
    float m_fovUpperBound;
    float m_znear;
    float m_zfar;
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_PERSPECTIVECAMERA_HPP

