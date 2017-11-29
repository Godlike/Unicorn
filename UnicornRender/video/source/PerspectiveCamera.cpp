/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/PerspectiveCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace unicorn
{
namespace video
{

PerspectiveCamera::PerspectiveCamera(system::Window* window, glm::mat4& cameraProj)
    : CameraProjection(window, cameraProj)
    , m_fov(45.f)
    , m_fovLowerBound(44.f)
    , m_fovUpperBound(45.f)
    , m_znear(0.1f)
    , m_zfar(100000.f) // 100 kilometers
{
    UpdateProjection();
}

void PerspectiveCamera::UpdateProjection()
{
    m_cameraProjection = glm::perspective(m_fov,
                                          m_aspect,
                                          m_znear,
                                          m_zfar);
}

void PerspectiveCamera::SetFov(float fov)
{
    m_fov = std::max(std::min(fov, m_fovUpperBound), m_fovLowerBound);
    UpdateProjection();
}

void PerspectiveCamera::SetZNear(float znear)
{
    m_znear = znear;
    UpdateProjection();
}

void PerspectiveCamera::SetZFar(float zfar)
{
    m_zfar = zfar;
    UpdateProjection();
}

float PerspectiveCamera::GetFov() const
{
    return m_fov;
}

} // namespace video
} // namespace unicorn
