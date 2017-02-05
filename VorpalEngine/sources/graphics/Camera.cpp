#include <vorpal/graphics/Camera.hpp>

vp::graphics::Camera::Camera()
    : m_fov(0.0f), m_znear(0.0f), m_zfar(0.0f), m_rotation(glm::vec3()), m_position(glm::vec3()), m_perspective(glm::mat4()), m_view(glm::mat4()), m_rotationSpeed(1.0f), m_movementSpeed(1.0f)
{
}
