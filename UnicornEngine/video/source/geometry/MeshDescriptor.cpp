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

void MeshDescriptor::Rotate(float angle, glm::vec3 axis)
{
    m_mesh->model = glm::rotate(m_mesh->model, angle, axis);
}

void MeshDescriptor::Move(glm::vec3 diff)
{
    auto newVertices = m_mesh->GetVertices();
    for (auto& vertex : newVertices)
    {
        vertex.pos += diff;
    }
    m_mesh->SetVertices(newVertices);
}

void MeshDescriptor::Scale(glm::vec3 diff)
{
    m_mesh->model = glm::scale(m_mesh->model, diff);
}

void MeshDescriptor::SetColor(glm::vec3 color)
{
    auto newVertices = m_mesh->GetVertices();
    for (auto& vertex : newVertices)
    {
        vertex.color = color;
    }
    m_mesh->SetVertices(newVertices);
}
}
}
}