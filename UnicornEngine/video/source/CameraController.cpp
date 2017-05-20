/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/CameraController.hpp>

namespace unicorn
{
namespace video
{
CameraController::CameraController(std::shared_ptr<Camera> camera)
    : m_camera(camera)
{
}

void CameraController::SetCameraProjection(ProjectionType newType)
{
    m_camera->SetProjectionType(newType);
}
}
}
