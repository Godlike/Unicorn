/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MODEL_HPP
#define UNICORN_VIDEO_MODEL_HPP

#include <unicorn/video/Material.hpp>
#include <unicorn/video/ModelMatrix.hpp>
#include <unicorn/utility/SharedMacros.hpp>

#include <list>

namespace unicorn
{
namespace video
{
class Mesh;

class Model
{
public:
    UNICORN_EXPORT Model(const std::list<Mesh*>& meshes, const Material& material);
    UNICORN_EXPORT Model(Mesh* mesh, const Material& material);
    UNICORN_EXPORT std::list<Mesh*> GetMeshes() const;
private:
    std::list<Mesh*> m_meshes;
    Material m_material;
    ModelMatrix m_modelMatrix;
};
}
}

#endif // UNICORN_VIDEO_GEOMETRY_MODEL_HPP
