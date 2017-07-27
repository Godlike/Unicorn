/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GEOMETRY_MODEL_HPP
#define UNICORN_VIDEO_GEOMETRY_MODEL_HPP


#include <unicorn/video/Material.hpp>
#include <unicorn/video/Texture.hpp>
#include <unicorn/video/geometry/Mesh.hpp>

namespace unicorn
{
namespace video
{
class Model
{
public:
    Model(const geometry::Mesh& mesh, const std::list<Texture>& textures, const Material& material);
private:
    geometry::Mesh m_mesh;
    std::list<Texture> m_textures;
    Material m_material;    
};
}
}

#endif // UNICORN_VIDEO_GEOMETRY_MODEL_HPP
