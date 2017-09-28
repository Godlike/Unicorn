/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERAPROJECTION_HPP
#define UNICORN_VIDEO_CAMERAPROJECTION_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/system/Window.hpp>
#include <glm/glm.hpp>

namespace unicorn
{
namespace video
{

/** @brief Camera projection abstract class for orthographical and perspetive projections */
class CameraProjection
{
public:
    /** @brief Updates camera projection */
    virtual void UpdateProjection() = 0;
protected:
    CameraProjection(system::Window* window, glm::mat4& cameraProj);
    virtual ~CameraProjection();

    /** @brief Disconnects from window signals */
    void Disconnect(system::Window*);

    /** @brief Recalculates aspect ratio on window size changed signal */
    void OnWindowSizeChanged(system::Window*, std::pair<int32_t, int32_t> windowSize);

    float m_aspect;
    glm::mat4& m_cameraProjection;
    system::Window* m_pWindow;
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERAPROJECTION_HPP

