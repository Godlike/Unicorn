/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MODEL_HPP
#define UNICORN_VIDEO_MODEL_HPP

#include <unicorn/video/Mesh.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>

namespace unicorn
{
namespace video
{

class Model
{
    bool LoadModel(std::string const& pathToModel);
public:
    std::string m_directory;
    std::vector<Mesh> m_meshes;

    void ProcessNode(aiNode* node, aiScene const* scene);
    // Mesh ProcessMesh(aiMesh* mesh, aiScene const* scene);
};
}
}

#endif // UNICORN_VIDEO_MODEL_HPP
