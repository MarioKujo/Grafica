#include "../Headers/AssimpMesh.hpp"
#include <iostream>

namespace udit
{
    // Constructor: carga el modelo desde archivo y genera buffers
    AssimpMesh::AssimpMesh(const std::string& filepath)
    {
        loadModel(filepath);
        generateBuffers();
    }

    // Carga el modelo con Assimp y procesa la primera malla
    void AssimpMesh::loadModel(const std::string& path)
    {
        Assimp::Importer importer;

        // Lee el archivo con flags para triangulación, invertir UVs y generar normales
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_GenNormals);

        // Verifica errores en la carga
        if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
        {
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        // Comprueba que existan mallas en el modelo
        if (scene->mNumMeshes == 0)
        {
            std::cerr << "ERROR::ASSIMP::No meshes found in file: " << path << std::endl;
            return;
        }

        // Procesa la primera malla del modelo
        processMesh(scene->mMeshes[0]);
    }

    // Procesa una malla: vértices y índices
    void AssimpMesh::processMesh(aiMesh* mesh)
    {
        processVertices(mesh);
        processIndices(mesh);
    }

    // Extrae vértices, normales y UVs de la malla
    void AssimpMesh::processVertices(aiMesh* mesh)
    {
        const bool hasNormals = mesh->HasNormals();
        const bool hasUVs = mesh->mTextureCoords[0] != nullptr;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            // Añade posición del vértice
            const auto& vertex = mesh->mVertices[i];
            data.coordinates.push_back(vertex.x);
            data.coordinates.push_back(vertex.y);
            data.coordinates.push_back(vertex.z);

            // Añade normales si existen
            if (hasNormals)
            {
                const auto& normal = mesh->mNormals[i];
                data.normals.push_back(normal.x);
                data.normals.push_back(normal.y);
                data.normals.push_back(normal.z);
            }

            // Añade UVs si existen, sino UVs a cero
            if (hasUVs)
            {
                const auto& uv = mesh->mTextureCoords[0][i];
                data.texCoords.push_back(uv.x);
                data.texCoords.push_back(uv.y);
            }
            else
            {
                data.texCoords.push_back(0.0f);
                data.texCoords.push_back(0.0f);
            }
        }
    }

    // Extrae los índices de los triángulos que forman la malla
    void AssimpMesh::processIndices(aiMesh* mesh)
    {
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            const auto& face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
            {
                data.indices.push_back(static_cast<GLuint>(face.mIndices[j]));
            }
        }
    }
}
