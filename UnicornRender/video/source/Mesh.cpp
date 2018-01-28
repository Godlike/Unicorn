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
Mesh::Mesh() :
    m_material(nullptr)
{
    auto const defaultMaterial = std::make_shared<Material>();
    SetMaterial(defaultMaterial);
}

Mesh::~Mesh()
{
    m_material->DataUpdated.clear();
}

void Mesh::SetMeshData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    m_vertices = vertices;
    m_indices = indices;

    VerticesUpdated.emit();
}

void Mesh::SetMaterial(std::shared_ptr<Material> material)
{
    // TODO: m_material->DataUpdated.clear();

    m_material = material;

    m_material->DataUpdated.connect(this, &Mesh::OnMaterialUpdated);

    MaterialUpdated.emit();
}

std::vector<Vertex> const& Mesh::GetVertices() const
{
    return m_vertices;
}

std::vector<uint32_t> const& Mesh::GetIndices() const
{
    return m_indices;
}

std::shared_ptr<Material> Mesh::GetMaterial() const
{
    return m_material;
}

void Mesh::OnMaterialUpdated()
{
    MaterialUpdated.emit();
}

}
}
