/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/geometry/Sphere.hpp>
#include <glm/gtc/constants.hpp>

namespace unicorn
{
    namespace video
    {
        namespace geometry
        {
            Sphere::Sphere(std::shared_ptr<Mesh> mesh, float radius, uint32_t rings, uint32_t sectors) : MeshDescriptor(mesh)
            {
                float const R = 1. / (float)(rings - 1);
                float const S = 1. / (float)(sectors - 1);
                int r, s;
                std::vector<Vertex> vertices;
                std::vector<uint16_t> indices;
                for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
                    float const y = sin(-glm::pi<float>() * 2 + glm::pi<float>() * r * R);
                    float const x = cos(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                    float const z = sin(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                    
                    Vertex vert;
                    vert.pos.x = x * radius;
                    vert.pos.y = y * radius;
                    vert.pos.z = z * radius;
                    vert.color = Color::White;  
                    vertices.push_back(vert);
                }

                indices.resize(rings * sectors * 4);
                auto i = indices.begin();
                for (r = 0; r < rings; r++)
                {
                    for (s = 0; s < sectors; s++) {
                        *i++ = r * sectors + s;
                        *i++ = r * sectors + (s + 1);
                        *i++ = (r + 1) * sectors + (s + 1);
                        *i++ = (r + 1) * sectors + s;
                    }
                }  
                m_mesh->SetMeshData(vertices, indices);
            }
        }
    }
}


