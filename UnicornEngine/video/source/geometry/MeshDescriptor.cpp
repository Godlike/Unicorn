/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/geometry/MeshDescriptor.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace unicorn
{
namespace video
{
namespace geometry
{
MeshDescriptor::MeshDescriptor(std::shared_ptr<Mesh> mesh)
    : m_mesh(mesh)
{
}

void MeshDescriptor::Rotate(float angle, const glm::vec3& axis)
{
    m_mesh->model = glm::rotate(m_mesh->model, angle, axis);
}

void MeshDescriptor::Move(const glm::vec3& translateVector)
{
    m_mesh->model = glm::translate(m_mesh->model, translateVector);
}

void MeshDescriptor::Scale(const glm::vec3& scaleVector)
{
    m_mesh->model = glm::scale(m_mesh->model, scaleVector);
}

MeshDescriptor::~MeshDescriptor()
{
}

void MeshDescriptor::SetColor(const glm::vec3& color)
{
    m_mesh->SetColor(color);
}
}
}
}
