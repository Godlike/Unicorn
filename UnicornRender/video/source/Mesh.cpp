/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Mesh.hpp>
#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace video
{
Mesh::Mesh() : name("DefaultName"), m_material(nullptr)

{
    SetMaterial(std::make_shared<Material>());
}

Mesh::~Mesh()
{
    m_material->DataUpdated.disconnect(this, &Mesh::OnMaterialUpdated);
}

void Mesh::SetMeshData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    m_vertices = vertices;
    m_indices = indices;

    VerticesUpdated.emit();
}

void Mesh::SetMaterial(std::shared_ptr<Material> material)
{
    if (material == nullptr)
    {
        LOG_WARNING("Can't set new material to mesh %s", name.c_str());
        return;
    }

    if (m_material != nullptr)
    {
        m_material->DataUpdated.disconnect(this, &Mesh::OnMaterialUpdated);
    }

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
