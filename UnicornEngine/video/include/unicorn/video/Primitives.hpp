/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_PRIMITIVES_HPP
#define UNICORN_VIDEO_PRIMITIVES_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/Mesh.hpp>

namespace unicorn
{
namespace video
{
class Cube : public Mesh
{
public:
    UNICORN_EXPORT Cube(Material& material);
private:
    //Hide it to prevent data updating
    void SetMeshData(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) = delete;
};

class Quad : public Mesh
{
public:
    UNICORN_EXPORT Quad(Material& material);
private:
    //Hide it to prevent data updating
    void SetMeshData(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) = delete;
};
}
}

#endif // UNICORN_VIDEO_PRIMITIVES_HPP
