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

/**
* @brief Camera struct, holds view and projection matrices.
*/
struct Camera
{
    glm::mat4 projection = glm::mat4();
    glm::mat4 view = glm::mat4();
};

/**
* @brief Camera controller class to create new controllers for cameras
*/
class CameraController
{
public:
    void SetDirection(glm::vec3 const& direction);
    void SetUpVector(glm::vec3 const& upVector);
    void SetPosition(glm::vec3 const& position);

    glm::vec3 const& GetDirection() const
    {
        return m_direction;
    }
    glm::vec3 const& GetUpVector() const
    {
        return m_upVector;
    }
    glm::vec3 const& GetPosition() const
    {
        return m_position;
    }
protected:
    CameraController(glm::mat4& cameraView);

    void UpdateViewMatrix();

    glm::mat4& m_cameraView;
    glm::vec3 m_position, m_upVector, m_direction;
    float speed;
};

/**
* @brief Camera projection abstract class for orthographical and perspetive projections
*/
class CameraProjection
{
protected:
    CameraProjection(system::Window* window, glm::mat4& cameraProj) :
        m_aspect(static_cast<float>(window->GetSize().first) / window->GetSize().second),
        m_cameraProjection(cameraProj), 
        m_pWindow(window)
    {
        m_pWindow->Destroyed.connect(this, &CameraProjection::Disconnect);
        m_pWindow->SizeChanged.connect(this, &CameraProjection::OnWindowSizeChanged);
    }

    virtual ~CameraProjection()
    {
        Disconnect(nullptr);
    }

    void Disconnect(system::Window*)
    {
        if (m_pWindow != nullptr)
        {
            m_pWindow->SizeChanged.disconnect(this, &CameraProjection::OnWindowSizeChanged);
            m_pWindow = nullptr;
        }
    }

    UNICORN_EXPORT void OnWindowSizeChanged(system::Window*, std::pair<int32_t, int32_t> windowSize)
    {
        UpdateProjection();
    }

    virtual void Scroll(float yoffset) = 0;
    virtual void UpdateProjection() = 0;

    float m_aspect;
    glm::mat4& m_cameraProjection;
    system::Window* m_pWindow;
};

class PerspectiveCamera : public CameraProjection
{
public:
    UNICORN_EXPORT PerspectiveCamera(system::Window* window, glm::mat4& cameraProj) : 
        CameraProjection(window, cameraProj),
        m_fovLowerBound(44.f),
        m_fovUpperBound(45.f),
        m_fov(45.f)
    {
        UpdateProjection();
    }

    void UpdateProjection() override final
    {
        m_cameraProjection = glm::perspective(m_fov,
                                              m_aspect,
                                              1.0f,
                                              1000.f);
    }

    void Scroll(float yoffset) override
    { 
        SetFov(m_fov - yoffset);
    }

    void SetFov(float fov)
    {
        m_fov = std::max(std::min(fov, m_fovUpperBound), m_fovLowerBound);
        UpdateProjection();
    }
private:
    float m_fov, m_fovLowerBound, m_fovUpperBound;
};

class OrthographicCamera : public CameraProjection
{
public:
    UNICORN_EXPORT OrthographicCamera(system::Window* window, glm::mat4& cameraProj) : CameraProjection(window, cameraProj), m_orthoScale(100.f), m_scaledAspect(m_aspect / m_orthoScale)
    {
        UpdateProjection();
    }

    void UpdateProjection() override final
    {
        m_cameraProjection = glm::ortho(-static_cast<float>(m_pWindow->GetSize().first) * m_scaledAspect, static_cast<float>(m_pWindow->GetSize().first) * m_scaledAspect,
                                        -static_cast<float>(m_pWindow->GetSize().second) * m_scaledAspect, static_cast<float>(m_pWindow->GetSize().second) * m_scaledAspect,
                                        -10000.0f, 10000.0f);
    }

    void Scroll(float yoffset) override
    {
        SetScale(m_orthoScale + yoffset);
    }

    void SetScale(float scale)
    {
        m_orthoScale = std::max(scale, 0.1f);
        m_scaledAspect = m_aspect / m_orthoScale;
        UpdateProjection();
    }

private:
    float m_orthoScale;
    float m_scaledAspect;
};

class Camera2DController : public CameraController
{
public:
    UNICORN_EXPORT Camera2DController(glm::mat4& cameraView) : CameraController(cameraView)
    {
    }

    UNICORN_EXPORT void MoveUp(float deltaTime)
    {
        m_position += glm::vec3(0.0f, -deltaTime * speed, 0.0f);
        UpdateViewMatrix();
    }

    UNICORN_EXPORT void MoveDown(float deltaTime)
    {
        m_position += glm::vec3(0.0f, speed * deltaTime, 0.0);
        UpdateViewMatrix();
    }
    UNICORN_EXPORT void MoveLeft(float deltaTime)
    {
        m_position -= glm::normalize(glm::cross(m_direction, m_upVector)) * speed * deltaTime; //TODO: optimize crossproduct
        UpdateViewMatrix();
    }
    UNICORN_EXPORT void MoveRight(float deltaTime)
    {
        m_position += glm::normalize(glm::cross(m_direction, m_upVector)) * speed * deltaTime;
        UpdateViewMatrix();;
    }
private:
};

/**
* @brief FPS style camera controller
*/
class CameraFpsController : public CameraController
{
public:
    UNICORN_EXPORT CameraFpsController(glm::mat4& cameraView);
    UNICORN_EXPORT void MoveUp(float deltaTime);
    UNICORN_EXPORT void MoveDown(float deltaTime);
    UNICORN_EXPORT void MoveLeft(float deltaTime);
    UNICORN_EXPORT void MoveRight(float deltaTime);
    UNICORN_EXPORT void MoveForward(float deltaTime);
    UNICORN_EXPORT void MoveBackward(float deltaTime);
    UNICORN_EXPORT void UpdateView(double posX, double posY);
    float sensitivity;
private:
    double m_lastX, m_lastY, m_yaw, m_pitch;
    bool m_dirty;
};

}
}

#endif // UNICORN_VIDEO_CAMERA_HPP
