/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Model.hpp>
#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace video
{

bool Model::LoadModel(std::string const& pathToModel)
{

    Assimp::Importer importer;

    aiScene const* scene = importer.ReadFile(pathToModel,
        aiProcess_Triangulate            |
        aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_ERROR("ERROR importing %s mesh : %s", pathToModel.c_str(),
            importer.GetErrorString());
        return false;
    }

    m_directory = pathToModel.substr(0, pathToModel.find_last_of('/'));

    //ProcessNode(scene->mRootNode, scene);

    return true;
}


void Model::ProcessNode(aiNode* node, aiScene const* scene)
{

}

// Mesh Model::ProcessMesh(aiMesh* mesh, aiScene const* scene)
// {
//     Mesh a;
//     return a;
// }

} // namespace video
} // namespace unicorn
