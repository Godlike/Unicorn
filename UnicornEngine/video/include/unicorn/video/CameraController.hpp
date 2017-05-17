/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERA_CONTROLLER_HPP
#define UNICORN_VIDEO_CAMERA_CONTROLLER_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/Camera.hpp>
#include <memory>

namespace unicorn
{
namespace video
{
/**
 * @brief Wrapper for camera, handle shared pointer to it.
 */
class CameraController
{
public:
    UNICORN_EXPORT CameraController(std::shared_ptr<Camera> camera);
    UNICORN_EXPORT void SetCameraProjection(ProjectionType newType);
protected:
    std::shared_ptr<Camera> m_camera;
};
}
}

#endif // UNICORN_VIDEO_CAMERA_CONTROLLER_HPP