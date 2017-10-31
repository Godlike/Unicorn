/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_PERSPECTIVECAMERA_HPP
#define UNICORN_VIDEO_PERSPECTIVECAMERA_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/CameraProjection.hpp>
#include <unicorn/system/Window.hpp>

namespace unicorn
{
namespace video
{

/** @brief Perspective camera projection controller */
class PerspectiveCamera final : public CameraProjection
{
public:
    UNICORN_EXPORT PerspectiveCamera(system::Window* window, glm::mat4& cameraProj);

    /** @brief Changes fov */
    UNICORN_EXPORT void Zoom(float zoomOffset);

    /** @brief Sets fov */
    UNICORN_EXPORT void SetFov(float fov);

    /** @brief Sets near Z plane */
    UNICORN_EXPORT void SetZNear(float znear);

    /** @brief Sets far Z plane */
    UNICORN_EXPORT void SetZFar(float zfar);

    /** @brief Returns fov */
    UNICORN_EXPORT float GetFov() const;

    /** @brief      { function_description } */
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

