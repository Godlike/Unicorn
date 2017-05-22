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
 * @brief Wrapper for camera, handle shared pointer to it
 */
class CameraController
{
public:
    /**
     * @brief Constructor
     * @param camera Which camera to control
     */
    UNICORN_EXPORT CameraController(std::shared_ptr<Camera> camera);
    UNICORN_EXPORT CameraController() = delete;
    /**
     * @brief Setter for new projection type
     * @param type New projection type
     */
    UNICORN_EXPORT void SetCameraProjection(ProjectionType type) const;
protected:
    std::shared_ptr<Camera> m_camera;
};
}
}

#endif // UNICORN_VIDEO_CAMERA_CONTROLLER_HPP
