/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Model.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/Material.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace unicorn
{
namespace video
{

bool Model::LoadModel(std::string const& pathToModel)
{

    Assimp::Importer importer;

    aiScene const* scene = importer.ReadFile(pathToModel,
        aiProcess_Triangulate |
        aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_ERROR("ERROR importing %s mesh : %s", pathToModel.c_str(),
            importer.GetErrorString());
        return false;
    }

    m_directory = pathToModel.substr(0, pathToModel.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);

    return true;
}


void Model::ProcessNode(aiNode* node, aiScene const* scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, aiScene const* scene)
{
    std::vector<uint32_t> indices;
    std::vector<Vertex> vertices;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;

        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tc = vec;
        }
        else
        {
            vertex.tc = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // if(mesh->mMaterialIndex >= 0)
    // {
    //     aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    //     vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    //     textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //     vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    //     textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // }


    Material mat;
    Mesh unicornMesh(mat);

    unicornMesh.SetMeshData(vertices, indices);

    return unicornMesh;
}

// vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
// {
//     vector<Texture> textures;
//     for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//     {
//         aiString str;
//         mat->GetTexture(type, i, &str);
//         Texture texture;
//         texture.id = TextureFromFile(str.C_Str(), directory);
//         texture.type = typeName;
//         texture.path = str;
//         textures.push_back(texture);
//     }
//     return textures;
// }

} // namespace video
} // namespace unicorn