/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace unicorn
{
namespace video
{

void CameraController::SetDirection(const glm::vec3& direction)
{
    m_direction = direction;
    m_rightVector = glm::normalize(glm::cross(m_direction, m_upVector));
    m_isDirty = true;
}

void CameraController::SetUpVector(const glm::vec3& upVector)
{
    m_upVector = upVector; 
    m_rightVector = glm::normalize(glm::cross(m_direction, m_upVector));
    m_isDirty = true;
}

void CameraController::SetPosition(const glm::vec3& position)
{
    m_position = position;
    m_isDirty = true;
}

glm::vec3 const& CameraController::GetDirection() const
{
    return m_direction;
}

glm::vec3 const& CameraController::GetUpVector() const
{
    return m_upVector;
}

glm::vec3 const& CameraController::GetPosition() const
{
    return m_position;
}

void CameraController::Frame()
{
    if (m_isDirty)
    {
        UpdateViewMatrix();
        m_isDirty = false;
    }
}

CameraController::CameraController(glm::mat4& cameraView) :
    m_cameraView(cameraView),
    m_position(0.0),
    m_upVector(0.0f, -1.0f, 0.0f),
    m_direction(0.0),
    m_rightVector(glm::cross(m_direction, m_upVector)),
    speed(100.f),
    m_isDirty(true)
{
}

void CameraController::UpdateViewMatrix()
{
    m_cameraView = glm::lookAt(m_position,
                               m_position + m_direction,
                               m_upVector);
}

CameraFpsController::CameraFpsController(glm::mat4& cameraView)
    : CameraController(cameraView),
    sensitivity(0.1f),
    m_lastX(0.0),
    m_lastY(0.0),
    m_yaw(90.0),
    m_pitch(0.0),
    m_isDirtyMousePosition(true)
{
}

void CameraFpsController::MoveUp(float deltaTime)
{
    m_position += glm::vec3(0.0f, -deltaTime * speed, 0.0f);
    m_isDirty = true;
}

void CameraFpsController::MoveDown(float deltaTime)
{
    m_position += glm::vec3(0.0f, speed * deltaTime, 0.0);
    m_isDirty = true;
}

void CameraFpsController::MoveLeft(float deltaTime)
{
    m_position -= m_rightVector * speed * deltaTime;
    m_isDirty = true;
}

void CameraFpsController::MoveRight(float deltaTime)
{
    m_position +=m_rightVector * speed * deltaTime;
    m_isDirty = true;
}

void CameraFpsController::MoveForward(float deltaTime)
{
    m_position += m_direction * speed * deltaTime;
    m_isDirty = true;
}

void CameraFpsController::MoveBackward(float deltaTime)
{
    m_position -= m_direction * speed * deltaTime;
    m_isDirty = true;
}

void CameraFpsController::UpdateView(double posX, double posY)
{
    if (!m_isDirtyMousePosition)
    {
        m_lastX = posX;
        m_lastY = posY;
        m_isDirtyMousePosition = true;
    }

    double xoffset = m_lastX - posX;
    double yoffset = posY - m_lastY;
    m_lastX = posX;
    m_lastY = posY;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    m_pitch = std::max(std::min(m_pitch, 89.0), -89.0);

    glm::vec3 front;
    front.x = static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
    front.y = static_cast<float>(sin(glm::radians(m_pitch)));
    front.z = static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
    m_direction = glm::normalize(front);
    m_rightVector = glm::normalize(glm::cross(m_direction, m_upVector));

    m_isDirty = true;
}

UNICORN_EXPORT void CameraFpsController::UpdateViewPositions(double posX, double posY)
{
    m_lastX = posX;
    m_lastY = posY;
}

CameraProjection::CameraProjection(system::Window* window, glm::mat4& cameraProj) :
    m_aspect(static_cast<float>(window->GetSize().first) / window->GetSize().second),
    m_cameraProjection(cameraProj),
    m_pWindow(window)
{
    m_pWindow->Destroyed.connect(this, &CameraProjection::Disconnect);
    m_pWindow->SizeChanged.connect(this, &CameraProjection::OnWindowSizeChanged);
}

CameraProjection::~CameraProjection()
{
    Disconnect(nullptr);
}

void CameraProjection::Disconnect(system::Window*)
{
    if (m_pWindow != nullptr)
    {
        m_pWindow->SizeChanged.disconnect(this, &CameraProjection::OnWindowSizeChanged);
        m_pWindow = nullptr;
    }
}

void CameraProjection::OnWindowSizeChanged(system::Window*, std::pair<int32_t, int32_t> size)
{
    m_aspect = static_cast<float>(size.first) / size.second;
    m_isDirty = true;
}

void CameraProjection::Frame()
{
    if (m_isDirty)
    {
        UpdateProjection();
    }
}

PerspectiveCamera::PerspectiveCamera(system::Window* window, glm::mat4& cameraProj) :
    CameraProjection(window, cameraProj),
    m_fov(45.f),
    m_fovLowerBound(44.f),
    m_fovUpperBound(45.f),
    m_znear(0.1f),
    m_zfar(1000.f)
{
}

void PerspectiveCamera::UpdateProjection()
{
    m_cameraProjection = glm::perspective(m_fov,
                                          m_aspect,
                                          m_znear,
                                          m_zfar);
}

void PerspectiveCamera::Scroll(float yoffset)
{
    SetFov(m_fov - yoffset);
}

void PerspectiveCamera::SetFov(float fov)
{
    m_fov = std::max(std::min(fov, m_fovUpperBound), m_fovLowerBound);
    m_isDirty = true;
}

UNICORN_EXPORT void PerspectiveCamera::SetZNear(float znear)
{
    m_znear = znear;
    m_isDirty = true;
}

UNICORN_EXPORT void PerspectiveCamera::SetZFar(float zfar)
{
    m_zfar = zfar;
    m_isDirty = true;
}

UNICORN_EXPORT float PerspectiveCamera::GetFov() const
{
    return m_fov;
}

OrthographicCamera::OrthographicCamera(system::Window* window, glm::mat4& cameraProj) :
    CameraProjection(window, cameraProj),
    m_orthoScale(100.f),
    m_scaledAspect(m_aspect / m_orthoScale),
    m_right(static_cast<float>(window->GetSize().first) / 2),
    m_top(static_cast<float>(m_pWindow->GetSize().second) / 2),
    m_back(-1000.f),
    m_front(1000.f)
{
}

void OrthographicCamera::UpdateProjection()
{
    m_right = static_cast<float>(m_pWindow->GetSize().first) / 2;
    m_top = static_cast<float>(m_pWindow->GetSize().second) / 2;

    float xScaled = m_right * m_scaledAspect;
    float yScaled = m_top * m_scaledAspect;

    m_cameraProjection = glm::ortho(-xScaled, xScaled,
                                    -yScaled, yScaled,
                                    m_back, m_front);
}

void OrthographicCamera::Scroll(float yoffset)
{
    SetScale(m_orthoScale + yoffset);
}

void OrthographicCamera::SetScale(float scale)
{
    m_orthoScale = std::max(scale, 0.1f);
    m_scaledAspect = m_aspect / m_orthoScale;
    m_isDirty = true;
}

UNICORN_EXPORT float OrthographicCamera::GetScale() const
{
    return m_orthoScale;
}

Camera2DController::Camera2DController(glm::mat4& cameraView) : CameraController(cameraView)
{
}

UNICORN_EXPORT void Camera2DController::MoveUp(float deltaTime)
{
    m_position += glm::vec3(0.0f, -deltaTime * speed, 0.0f);
    m_isDirty = true;
}

UNICORN_EXPORT void Camera2DController::MoveDown(float deltaTime)
{
    m_position += glm::vec3(0.0f, speed * deltaTime, 0.0);
    m_isDirty = true;
}

UNICORN_EXPORT void Camera2DController::MoveLeft(float deltaTime)
{
    m_position -= m_rightVector * speed * deltaTime;
    m_isDirty = true;
}

UNICORN_EXPORT void Camera2DController::MoveRight(float deltaTime)
{
    m_position += m_rightVector * speed * deltaTime;
    m_isDirty = true;
}

}
}
