/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GEOMETRY_TRIANGLE_HPP
#define UNICORN_VIDEO_GEOMETRY_TRIANGLE_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/geometry/Mesh.hpp>

#include <vector>

namespace unicorn
{
    namespace video
    {
        namespace geometry
        {
            class Triangle
            {
            public:
                UNICORN_EXPORT Triangle(Mesh* mesh);
                UNICORN_EXPORT void SetVertices(const std::vector<Vertex>& verts);
            private:
                Mesh* m_mesh;
            };
        }
    }
}

#endif // UNICORN_VIDEO_GEOMETRY_TRIANGLE_HPP