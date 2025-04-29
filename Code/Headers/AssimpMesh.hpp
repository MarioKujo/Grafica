#pragma once
#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace udit
{
    class AssimpMesh : public Mesh
    {
    public:
        AssimpMesh(const std::string& filepath);
    private:
        void loadModel(const std::string& path);
        void processMesh(aiMesh* mesh);
    };
}
