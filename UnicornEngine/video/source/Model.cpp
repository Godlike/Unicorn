/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Model.hpp>

namespace unicorn
{
namespace video
{
unicorn::video::Model::Model(const std::list<Mesh*>& meshes, const Material& material)
    : m_meshes(meshes)
    , m_material(material)
{
}

Model::Model(Mesh* mesh, const Material& material) : m_material(material)
{
    m_meshes.push_back(mesh);
}

std::list<Mesh*> Model::GetMeshes() const
{
    return m_meshes;
}
}
}
