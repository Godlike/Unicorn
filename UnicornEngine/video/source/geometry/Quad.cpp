/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/geometry/Quad.hpp>

namespace unicorn
{
    namespace video
    {
        namespace geometry
        {

            Quad::Quad(std::shared_ptr<Mesh> mesh) : MeshDescriptor(mesh)
            {
                m_mesh->vertices = {
                    { { -1.0f, -1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f } },
                    { { 1.0f, -1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f } },
                    { { 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f } },
                    { { -1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f } }
                };
                m_mesh->indices = {
                    0, 1, 2, 2, 3, 0
                };
                m_mesh->Updated();
            }
        }
    }
}