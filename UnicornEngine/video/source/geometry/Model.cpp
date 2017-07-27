/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/geometry/Model.hpp>

namespace unicorn
{
namespace video
{
    unicorn::video::Model::Model(const geometry::Mesh& mesh, const std::list<Texture>& textures, const Material& material) 
        : m_mesh(mesh), 
          m_textures(textures),
          m_material(material)
    {

    }
}
}
