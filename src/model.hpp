#pragma once
#ifndef MODEL_HPP
#define MODEL_HPP


#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "mesh.hpp"
#include "texture.hpp"
#include "shader.hpp"

#include <vector>
#include <filesystem>
#include <fstream>
#include <map>

class Model
{
private:
    
    std::vector<Mesh> _meshes;
    std::filesystem::path _path;

    static std::filesystem::path directory;
    std::ofstream model_log;

    Texture _diffuse;
    Texture _specular;

    glm::mat4 convertMatrix(aiMatrix4x4 mat)
    {
        return glm::mat4(
            mat.a1, mat.a2, mat.a3, mat.a4,
            mat.b1, mat.b2, mat.b3, mat.b4,
            mat.c1, mat.c2, mat.c3, mat.c4,
            mat.d1, mat.d2, mat.d3, mat.d4
        );
    }
public:

    Model(const std::filesystem::path &path, bool standart_dir=true);

    void process_node(aiNode* node, const aiScene* scene, const glm::mat4& trans = glm::mat4(1.f));
    void process_mesh(aiMesh* mesh, const aiScene* scene,  const glm::mat4& trans);

    void draw(
        const Shader& shader,
        const glm::mat4& view,
        const glm::mat4& projection) const
    {
        shader.use();
        _diffuse.bind();
        shader.setMat4("model", glm::mat4(1.f));
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        for (const auto& mesh : _meshes)
        {
            mesh.draw();
        }
    }
    ~Model() = default;
};

Model::Model(const std::filesystem::path& path, bool standart_dir)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(directory / path, 
    aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        throw std::runtime_error(std::string("ERROR::ASSIMP::") + import.GetErrorString() + '\n');
    }
    model_log.open("model.log", std::ios_base::trunc);

    _diffuse.init("../resources/models/backpack/diffuse.jpg", false, false);
    _specular.init("../resources/models/backpack/specular.jpg", false, false);

    auto* rootNode = scene->mRootNode;
    process_node(rootNode, scene, glm::mat4(1.f));

    model_log.close();
}

void Model::process_node(aiNode* node, const aiScene* scene, const glm::mat4& trans)
{
    for (size_t meshNum = 0; meshNum < node->mNumMeshes; ++meshNum)
        process_mesh(scene->mMeshes[node->mMeshes[meshNum]], scene, trans);
    
    for(size_t childNum = 0; childNum < node->mNumChildren; ++childNum)
        process_node( node->mChildren[childNum], scene, glm::mat4(1.f));
}

void Model::process_mesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& trans)
{
    model_log << "Mesh\n";

    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        
        Vertex vertex;

        model_log << "pos: " << i;
        model_log << " x: " << (vertex._pos.x = mesh->mVertices[i].x);
        model_log << " y: " << (vertex._pos.y = mesh->mVertices[i].y);
        model_log << " z: " << (vertex._pos.z = mesh->mVertices[i].z);
        model_log << '\n';

        if (mesh->HasNormals())
        {
            vertex._nor.x = mesh->mNormals[i].x;
            vertex._nor.y = mesh->mNormals[i].y;
            vertex._nor.z = mesh->mNormals[i].z;
        }

        //checks if mesh has texture coords
        if(mesh->mTextureCoords[0])
        {
           vertex._tex.x = mesh->mTextureCoords[0][i].x;
           vertex._tex.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }


    std::vector<GLuint> indices;
    indices.reserve(mesh->mNumFaces * 3);
    
    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        
        indices.push_back(mesh->mFaces[i].mIndices[0]);
        indices.push_back(mesh->mFaces[i].mIndices[1]);
        indices.push_back(mesh->mFaces[i].mIndices[2]);

        model_log << indices[i+0] << " "; 
        model_log << indices[i+1] << " "; 
        model_log << indices[i+2] << " ";
        model_log << '\n';
    }

    _meshes.push_back(Mesh(vertices, indices));
    //_meshes.back()._trans = trans;
}

std::filesystem::path Model::directory = "../resources/models/";

#endif