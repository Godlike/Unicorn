/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_ORTHOGRAPHICCAMERA_HPP
#define UNICORN_VIDEO_ORTHOGRAPHICCAMERA_HPP

#include <unicorn/video/CameraProjection.hpp>
#include <unicorn/system/Window.hpp>

namespace unicorn
{
namespace video
{

static float constexpr BACK_ORTHO_FRUSTRUM_PLANE = -1000.f;
static float constexpr FRONT_ORTHO_FRUSTRUM_PLANE = 1000.f;
static float constexpr DEFAULT_ORTHO_BOX_SCALE = 100.f;

/** @brief Orthographic camera projection controller */
class OrthographicCamera final : public CameraProjection
{
public:
    /**
    * @brief Creates orthographics camera projection
    * @param[in] window
    * @param[in] cameraProj projection matrix
    */
    OrthographicCamera(system::Window& window, glm::mat4& cameraProj);

    /** @brief Recalculates projection */
    void UpdateProjection() override;

    /** @brief Sets orthogonal box scale factors */
    void SetScale(float scale);

    /** @brief Returns scale */
    float GetScale() const;
private:
    float m_orthoScale;
    float m_scaledAspect;
    float m_right;
    float m_top;
    float m_back;
    float m_front;
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_ORTHOGRAPHICCAMERA_HPP
