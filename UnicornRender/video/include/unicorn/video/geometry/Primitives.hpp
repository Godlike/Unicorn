/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GEOMETRY_PRIMITIVES_HPP
#define UNICORN_VIDEO_GEOMETRY_PRIMITIVES_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/geometry/Mesh.hpp>
#include <unicorn/video/geometry/MeshDescriptor.hpp>

namespace unicorn
{
namespace video
{
namespace geometry
{

/** @brief  Fills meshes with geometric primitive's data */
class Primitives
{
public:
    /**
     *  @brief Fills given mesh with triangle data and returns mesh descriptor for it
     *
     *  @param  mesh    geometry mesh
     *
     *  @return MeshDescriptor for given mesh
     */
    UNICORN_EXPORT static MeshDescriptor Triangle(Mesh& mesh);

    /**
     *  @brief Fills given mesh with cube data and returns mesh descriptor for it
     *
     *  @param  mesh    geometry mesh
     *
     *  @return MeshDescriptor for given mesh
     */
    UNICORN_EXPORT static MeshDescriptor Cube(Mesh& mesh);

    /**
     *  @brief Fills given mesh with quad data and returns mesh descriptor for it
     *
     *  @param  mesh    geometry mesh
     *
     *  @return MeshDescriptor for given mesh
     */
    UNICORN_EXPORT static MeshDescriptor Quad(Mesh& mesh);
};

}
}
}

#endif // UNICORN_VIDEO_GEOMETRY_PRIMITIVES_HPP
