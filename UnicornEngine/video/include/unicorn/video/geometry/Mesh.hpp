/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GEOMETRY_MESH_HPP
#define UNICORN_VIDEO_GEOMETRY_MESH_HPP

#include <unicorn/utility/SharedMacros.hpp>

#include <glm/glm.hpp>
#include <vector>

namespace unicorn
{
    namespace video
    {
        namespace geometry
        {
            struct Vertex {
                glm::vec2 pos;
                glm::vec3 color;
            };

            class Mesh
            {
            public:
				UNICORN_EXPORT Mesh();
                UNICORN_EXPORT void SetVertices(const std::vector<Vertex>& vertices)
                {
                    m_vertices = vertices;
                }
            private:
                glm::mat4 m_model; 
                std::vector<Vertex> m_vertices;
            };
        }
    }
}

#endif // UNICORN_VIDEO_GEOMETRY_MESH_HPP