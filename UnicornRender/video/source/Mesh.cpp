/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Mesh.hpp>

namespace unicorn
{
namespace video
{
Mesh::Mesh(Material const& material)
    : m_material(material)
    , m_scale(glm::vec3(1.f))
{
}

void Mesh::SetMeshData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    m_vertices = vertices;
    m_indices = indices;

    VerticesUpdated.emit();
}

void Mesh::SetMaterial(Material const& material)
{
    m_material = material;

    MaterialUpdated.emit();
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
    return m_vertices;
}

const std::vector<uint32_t>& Mesh::GetIndices() const
{
    return m_indices;
}

const Material& Mesh::GetMaterial() const
{
    return m_material;
}

void Mesh::Scale(glm::vec3 scale)
{
    m_scale = scale;
}

void Mesh::Update()
{
    UpdateOrientation();

    auto T = glm::translate(glm::mat4(1.0), m_translation);
    auto R = mat4_cast(m_orientation) * glm::mat4(1.0);
    auto S = scale(glm::mat4(1.0), { m_scale });

    m_transformMatrix = T * R * S;
}

}
}
