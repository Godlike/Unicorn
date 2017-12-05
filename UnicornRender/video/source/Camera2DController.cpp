/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Camera2DController.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace unicorn
{
namespace video
{

Camera2DController::Camera2DController(glm::mat4& cameraView) :
    m_cameraView(cameraView)
{
}

void Camera2DController::Update()
{
    UpdateModelMatrix();
    m_cameraView = glm::lookAt(m_translation, m_translation + m_direction, m_upVector);
}

} // namespace video
} // namespace unicorn
