/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GEOMETRY_CUBE_HPP
#define UNICORN_VIDEO_GEOMETRY_CUBE_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/geometry/Mesh.hpp>
#include <unicorn/video/geometry/MeshDescriptor.hpp>

#include <memory>

namespace unicorn
{
    namespace video
    {
        namespace geometry
        {
            class Cube : public MeshDescriptor
            {
            public: 
                UNICORN_EXPORT Cube(std::shared_ptr<Mesh> mesh);
            };
        }
    }
}

#endif // UNICORN_VIDEO_GEOMETRY_CUBE_HPP