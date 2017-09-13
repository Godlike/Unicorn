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

class PerspectiveCamera
{
public:
    UNICORN_EXPORT PerspectiveCamera(system::Window* window, glm::mat4& cameraProj) : m_cameraProjection(cameraProj), m_pWindow(window)
    {
        m_pWindow->SizeChanged.connect(this, &unicorn::video::PerspectiveCamera::OnWindowSizeChanged);
        float aspect = static_cast<float>(m_pWindow->GetSize().first) / m_pWindow->GetSize().second;
        m_cameraProjection = glm::perspective(45.0f,
                                              aspect,
                                              1.0f,
                                              1000.f);
    }

    UNICORN_EXPORT ~PerspectiveCamera()
    {
        //m_pWindow->SizeChanged.disconnect(this, &unicorn::video::PerspectiveCamera::OnWindowSizeChanged);
    }

    UNICORN_EXPORT void OnWindowSizeChanged(system::Window*, std::pair<int32_t, int32_t> windowSize)
    {
        m_cameraProjection = glm::perspective(45.0f,
            static_cast<float>(windowSize.first) / windowSize.second,
                                              1.0f,
            1000.f);
    }
private:
    glm::mat4& m_cameraProjection;
    system::Window* m_pWindow;
};

class OrthographicCamera
{
public:
    UNICORN_EXPORT OrthographicCamera(system::Window* window, glm::mat4& cameraProj) : m_cameraProjection(cameraProj), m_pWindow(window)
    {
        m_pWindow->SizeChanged.connect(this, &unicorn::video::OrthographicCamera::OnWindowSizeChanged);
        m_cameraProjection = glm::ortho(0.0f, static_cast<float>(m_pWindow->GetSize().first),
                                        static_cast<float>(m_pWindow->GetSize().second), 0.0f,
                                        -100.0f, 10000.0f);
    }

    UNICORN_EXPORT ~OrthographicCamera()
    {
        //m_pWindow->SizeChanged.disconnect(this, &unicorn::video::OrthographicCamera::OnWindowSizeChanged);
    }

    UNICORN_EXPORT void OnWindowSizeChanged(system::Window*, std::pair<int32_t, int32_t> windowSize)
    {
        m_cameraProjection = glm::ortho(0.0f, static_cast<float>(windowSize.first), 
                                        static_cast<float>(windowSize.second), 0.0f,
                                        -100.0f, 10000.0f);
    }
private:
    glm::mat4& m_cameraProjection;
    system::Window* m_pWindow;
};

/**
 * @brief Camera class, holds view and projection matrices.
 */
class Camera
{
public:
    UNICORN_EXPORT Camera();

    /**
     * @brief Changes camera position by given vector
     * @param delta Vector of translation
     */
    UNICORN_EXPORT void Translate(glm::vec3 const& delta);

    /**
     * @brief Sets direction
     * @param direction New direction value
     */
    UNICORN_EXPORT void SetDirection(glm::vec3 const& direction);

    /**
     * @brief Sets up vector
     * @param upVector New up vector value
     */
    UNICORN_EXPORT void SetUpVector(glm::vec3 const& upVector);

    /**
     * @brief Sets position
     * @param position New position value
     */
    UNICORN_EXPORT void SetPosition(glm::vec3 const& position);

    /**
     * @brief Returns direction
     * @return Direction vector
     */
    UNICORN_EXPORT const glm::vec3& GetDirection() const;

    /**
     * @brief Returns up vector
     * @return Up vector value
     */
    UNICORN_EXPORT const glm::vec3& GetUpVector() const;

    /**
     * @brief Returns view matrix
     * @return View matrix value
     */
    UNICORN_EXPORT const glm::mat4& GetView() const;

    /**
    * @brief Checks if view or projection need to be updated and update it, if needed
    */
    UNICORN_EXPORT void Frame();

    glm::mat4 projection = glm::mat4();
    glm::mat4 view;

private:
    float m_aspect;
    glm::vec3 m_position;
    glm::vec3 m_upVector;
    glm::vec3 m_direction;

    bool m_dirtyView;
    /**
     * @brief Calculates view matrix
     */
    void UpdateViewMatrix();
};
}
}

#endif // UNICORN_VIDEO_CAMERA_HPP
