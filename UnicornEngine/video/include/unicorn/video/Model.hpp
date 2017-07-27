/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MODEL_HPP
#define UNICORN_VIDEO_MODEL_HPP

#include <unicorn/video/Material.hpp>
#include <unicorn/video/ModelMatrix.hpp>

namespace unicorn
{
namespace video
{
class Mesh;

class Model
{
public:
    Model(const std::list<Mesh*>& meshes, const Material& material);
    Model(const Mesh& mesh, const Material& material);
private:
    std::list<Mesh*> m_meshes;
    Material m_material;
    ModelMatrix m_modelMatrix;
};
}
}

#endif // UNICORN_VIDEO_GEOMETRY_MODEL_HPP
