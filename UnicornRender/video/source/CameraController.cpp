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

void CameraController::Calculate()
{
    Update();
    m_cameraView = m_transformMatrix;
}

CameraController::CameraController(glm::mat4& cameraView)
    : m_cameraView(cameraView)
{

}

} // namespace video
} // namespace unicorn
