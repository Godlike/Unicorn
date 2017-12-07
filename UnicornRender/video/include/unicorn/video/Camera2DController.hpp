/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERA2DCONTROLLER_HPP
#define UNICORN_VIDEO_CAMERA2DCONTROLLER_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/Transform.hpp>

namespace unicorn
{
namespace video
{

/** @brief 2D camera controller */
class Camera2DController : public Transform
{
public:
    /**
     * @brief Constructs camera 2d controller
     *
     * @param[out] cameraView target camera view matix
     */
    UNICORN_EXPORT Camera2DController(glm::mat4& cameraView);

    UNICORN_EXPORT void Update();
private:
    glm::mat4& m_cameraView;
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERA2DCONTROLLER_HPP
