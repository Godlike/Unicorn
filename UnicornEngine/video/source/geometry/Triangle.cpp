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
            Triangle::Triangle(const std::vector<Vertex>& verts, std::shared_ptr<Mesh> mesh) : m_mesh(mesh)
            {
                m_mesh->SetVertices(verts);
            }

        }
    }
}

