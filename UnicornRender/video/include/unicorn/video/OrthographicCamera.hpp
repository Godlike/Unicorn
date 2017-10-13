/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_ORTHOGRAPHICCAMERA_HPP
#define UNICORN_VIDEO_ORTHOGRAPHICCAMERA_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/CameraProjection.hpp>
#include <unicorn/system/Window.hpp>

namespace unicorn
{
namespace video
{

/** @brief Orthographic camera projection controller */
class OrthographicCamera final : public CameraProjection
{
public:
    UNICORN_EXPORT OrthographicCamera(system::Window* window, glm::mat4& cameraProj);

    UNICORN_EXPORT void UpdateProjection() override;

    /** @brief Changes scale by adding scaleOffset to it */
    UNICORN_EXPORT void Scale(float scaleOffset);

    /** @brief Sets scale which scaling ortho box */
    UNICORN_EXPORT void SetScale(float scale);

    /** @brief Returns scale */
    UNICORN_EXPORT float GetScale() const;
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
