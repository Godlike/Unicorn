/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERAFPSCONTROLLER_HPP
#define UNICORN_VIDEO_CAMERAFPSCONTROLLER_HPP

#include <unicorn/video/Transform.hpp>

namespace unicorn
{
namespace video
{

/** @brief FPS style camera controller */
class CameraFpsController : public Transform
{
public:
    /**
     * @brief Constructs camera fps controller
     *
     * @param[out] cameraView reference to camera view
     */
    CameraFpsController(glm::mat4& cameraView);

    /** @brief Updates view matrix by taking diff between past and new mouse coordinates */
    void UpdateView(float x, float y);

    /** @brief Sets mouse coordinates without updating view matrix */
    void SetViewPositions(float x, float y);

    /** @brief Sets flag to update mouse position in next UpdateView call
     *
     *  Mouse position updating without orientation recalculation
     */
    void ResetView();

    /** @brief Calculates and updates camera view matrix */
    void Update();
private:
    /** @brief Calculates orientation in space */
    virtual void UpdateOrientation() override;

    glm::vec2 m_mousePosition;
    bool m_dirtyViewPosition;

    glm::mat4& m_cameraView;
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERAFPSCONTROLLER_HPP
