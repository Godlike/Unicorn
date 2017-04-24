/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/geometry/Triangle.hpp>

namespace unicorn
{
    namespace video
    {
        namespace geometry
        {
            Triangle::Triangle(std::shared_ptr<Mesh> mesh) : m_mesh(mesh)
            {
                std::vector<Vertex> verts = {
                    { { 0.0f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
                    { { 0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f } },
                    { { -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } }
                };
                SetVertices(verts);
            }

            void Triangle::SetVertices(const std::vector<Vertex>& verts)
            {
                m_mesh->m_vertices = verts;
                m_mesh->m_dataUpdated.emit();
            }
        }
    }
}

