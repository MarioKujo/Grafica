#pragma once

#include <vector>
#include <string>
#include <glm.hpp>
#include <glad/glad.h>

class Heightmap {
public:
    Heightmap(const std::string& heightmapPath, float width, float height, float maxHeight);
    ~Heightmap();

    void render() const;
    GLuint getTextureID() const { return textureID; }

private:
    GLuint vao, vbo, ebo;
    GLuint textureID;
    unsigned int numIndices;

    void generateMesh(const std::vector<float>& heightData, int width, int height, float maxHeight);
    std::vector<float> loadHeightData(const std::string& heightmapPath, int& width, int& height);
};
