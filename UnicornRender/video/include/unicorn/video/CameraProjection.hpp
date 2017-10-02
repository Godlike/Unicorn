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
    /**
    * @brief Constructs CameraProjection
    * @param [in] window holds pointer to window to update aspect ratio
    * @param [in] cameraProj camera projection matrix
    */
    CameraProjection(system::Window* window, glm::mat4& cameraProj);
    /** @brief Disconnects from window signals */
    virtual ~CameraProjection();

    /** @brief Recalculates projection matrix */
    virtual void UpdateProjection() = 0;
protected:
    /** @brief Disconnects from window signals */
    void OnWindowDestroyed(system::Window*);

    /** @brief Recalculates aspect ratio */
    void OnWindowSizeChanged(system::Window*, std::pair<int32_t, int32_t> windowSize);

    //! Aspect ratio
    float m_aspect;
    //! Reference to camera projection matrix
    glm::mat4& m_cameraProjection;
    //! Pointer to window
    system::Window* m_pWindow;
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERAPROJECTION_HPP

