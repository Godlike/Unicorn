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

/** @brief Camera struct, holds view and projection matrices. */
struct Camera
{
    glm::mat4 projection = glm::mat4();
    glm::mat4 view = glm::mat4();
};

/** @brief Camera controller class to create new controllers for cameras */
class CameraController
{
public:
    /** @brief Sets direction of camera */
    UNICORN_EXPORT void SetDirection(glm::vec3 const& direction);

    /** @brief Sets up vector of camera */
    UNICORN_EXPORT void SetUpVector(glm::vec3 const& upVector);

    /** @brief Sets position of camera */
    UNICORN_EXPORT void SetPosition(glm::vec3 const& position);

    /** @brief Returns direction of camera */
    UNICORN_EXPORT glm::vec3 const& GetDirection() const;

    /** @brief Returns up vector of camera */
    UNICORN_EXPORT glm::vec3 const& GetUpVector() const;

    /** @brief Returns position of camera */
    UNICORN_EXPORT glm::vec3 const& GetPosition() const;

    /** @brief Checks if camera view matrix must be updated and do it */
    UNICORN_EXPORT void Frame();
protected:
    CameraController(glm::mat4& cameraView);

    /** @brief Updates view matrix */
    void UpdateViewMatrix();

    glm::mat4& m_cameraView;
    glm::vec3 m_position, m_upVector, m_direction;
    float speed;

    bool m_isDirty;
};

/** @brief Camera projection abstract class for orthographical and perspetive projections */
class CameraProjection
{
public:
    /** @brief Checks if camera projection matrix must be updated and do it */
    UNICORN_EXPORT void Frame();
protected:
    CameraProjection(system::Window* window, glm::mat4& cameraProj);
    virtual ~CameraProjection();

    /** @brief Disconnects from windows signals */
    void Disconnect(system::Window*);

    /** @brief Recalculates aspect ration on window size changed signal */
    void OnWindowSizeChanged(system::Window*, std::pair<int32_t, int32_t> windowSize);

    /** @brief Wheel scroll action */
    virtual void Scroll(float yoffset) = 0;
    virtual void UpdateProjection() = 0;

    float m_aspect;
    glm::mat4& m_cameraProjection;
    system::Window* m_pWindow;
    bool m_isDirty;
};

/** @brief Perspective camera projection controller */
class PerspectiveCamera : public CameraProjection
{
public:
    UNICORN_EXPORT PerspectiveCamera(system::Window* window, glm::mat4& cameraProj);

    /** @brief Changes fov */
    UNICORN_EXPORT void Scroll(float yoffset) override;

    /** @brief Sets fov */
    UNICORN_EXPORT void SetFov(float fov);

    /** @brief Returns fov */
    UNICORN_EXPORT float GetFov() const;
private:
    float m_fov, m_fovLowerBound, m_fovUpperBound, m_znear, m_zfar;

    void UpdateProjection() override final;
};

/** @brief Orthographic camera projection controller */
class OrthographicCamera : public CameraProjection
{
public:
    UNICORN_EXPORT OrthographicCamera(system::Window* window, glm::mat4& cameraProj);
    
    UNICORN_EXPORT void UpdateProjection() override final;
    UNICORN_EXPORT void Scroll(float yoffset) override;
    UNICORN_EXPORT void SetScale(float scale);

    UNICORN_EXPORT float GetScale() const;
private:
    float m_orthoScale;
    float m_scaledAspect;
    float m_right, m_top;
    float m_back, m_front;
};

/** @brief 2D camera controller */
class Camera2DController : public CameraController
{
public:
    UNICORN_EXPORT Camera2DController(glm::mat4& cameraView);

    UNICORN_EXPORT void MoveUp(float deltaTime);
    UNICORN_EXPORT void MoveDown(float deltaTime);
    UNICORN_EXPORT void MoveLeft(float deltaTime);
    UNICORN_EXPORT void MoveRight(float deltaTime);
private:
};

/** @brief FPS style camera controller */
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

    /** @brief Updates view matrix by taking diff between past and new mouse coordinates */
    UNICORN_EXPORT void UpdateView(double posX, double posY);

    /** @brief Sets mouse coordinates without updating view matrix */
    UNICORN_EXPORT void UpdateViewPositions(double posX, double posY);

    //! Sensitivity of mouse
    float sensitivity;
private:
    double m_lastX, m_lastY, m_yaw, m_pitch;
    bool m_isDirtyMousePosition;
};

}
}

#endif // UNICORN_VIDEO_CAMERA_HPP
