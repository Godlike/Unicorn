/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/geometry/MeshDescriptor.hpp>

namespace unicorn
{
    namespace video
    {
        namespace geometry
        {
            void MeshDescriptor::Rotate(Axis axis, float angle)
            {
            }

            void MeshDescriptor::Move(glm::vec3 diff)
            {
                for (auto& vertex : m_mesh->vertices)
                {
                    vertex.pos += diff;
                }
                m_mesh->Updated();
            }

            void MeshDescriptor::Scale(glm::vec3 diff)
            {
            }

            void MeshDescriptor::SetColor(glm::vec3 color)
            {
                for(auto& vertex : m_mesh->vertices)
                {
                    vertex.color = color;
                }
                m_mesh->Updated();
            }
        }
    }
}