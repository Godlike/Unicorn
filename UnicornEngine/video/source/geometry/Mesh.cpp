/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/geometry/Mesh.hpp>

namespace unicorn
{
namespace video
{
namespace geometry
{
Mesh::Mesh() : model(glm::mat4())
{
}

Mesh::~Mesh()
{
}

void Mesh::SetColor(const glm::vec3& ñolor)
{
    for (auto& x : m_vertices)
    {
        x.color = ñolor;
    }
    VerticesUpdated.emit();
}

void Mesh::SetMeshData(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
    m_vertices = vertices;
    m_indices = indices;
    VerticesUpdated.emit();
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
    return m_vertices;
}

const std::vector<uint16_t>& Mesh::GetIndices() const
{
    return m_indices;
}

Vertex::Vertex(glm::vec3 pos, glm::vec3 color): pos(pos)
                                              , color(color)
{
}
}
}
}
