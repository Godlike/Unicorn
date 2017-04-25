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
                const std::vector<uint16_t> indices = {
                    0, 1, 2
                };
                m_mesh->SetDrawData(verts, indices);
            }

        }
    }
}

