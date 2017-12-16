/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MODEL_HPP
#define UNICORN_VIDEO_MODEL_HPP

#include <unicorn/video/Mesh.hpp>

#include <string>

class aiNode;
class aiScene;
class aiMesh;
class aiMaterial;

namespace unicorn
{
namespace video
{

class Model
{
public:
    bool LoadModel(std::string const& pathToModel);
    std::vector<Mesh*> m_meshes;
private:
    std::string m_directory;

    void ProcessNode(aiNode* node, aiScene const* scene);
    Mesh* ProcessMesh(aiMesh* mesh, aiScene const* scene);
};
}
}

#endif // UNICORN_VIDEO_MODEL_HPP
