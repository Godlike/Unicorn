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
Mesh::Mesh(Material& material) : m_material(&material)
{
}

Mesh::~Mesh()
{
}

void Mesh::SetMeshData(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
    m_vertices = vertices;
    m_indices = indices;

    VerticesUpdated.emit();
}

void Mesh::SetMaterial(Material& material)
{
    m_material = &material;

    MaterialUpdated.emit(this);
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
    return m_vertices;
}

const std::vector<uint16_t>& Mesh::GetIndices() const
{
    return m_indices;
}

const Material& Mesh::GetMaterial() const
{
    return *m_material;
}

Vertex::Vertex(glm::vec3 pos, glm::vec2 tc) : pos(pos)
                                            , tc(tc)
{
}
}
}
