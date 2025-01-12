#include "../Headers/Heightmap.hpp"
#include "../Headers/stb_image.hpp"
#include <iostream>

Heightmap::Heightmap(const std::string& heightmapPath, float width, float height, float maxHeight)
    : vao(0), vbo(0), ebo(0), textureID(0), numIndices(0) {
    int imgWidth, imgHeight;
    auto heightData = loadHeightData(heightmapPath, imgWidth, imgHeight);
    if (!heightData.empty()) {
        generateMesh(heightData, imgWidth, imgHeight, maxHeight);
    }
}

Heightmap::~Heightmap() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
}

std::vector<float> Heightmap::loadHeightData(const std::string& heightmapPath, int& width, int& height) {
    int channels;
    unsigned char* data = stbi_load(heightmapPath.c_str(), &width, &height, &channels, 1); // Load as grayscale
    if (!data) {
        std::cerr << "Failed to load heightmap: " << heightmapPath << std::endl;
        return {};
    }

    std::vector<float> heightData(width * height);
    for (int i = 0; i < width * height; ++i) {
        heightData[i] = data[i] / 255.0f; // Normalize height to [0, 1]
    }

    stbi_image_free(data);
    return heightData;
}

void Heightmap::generateMesh(const std::vector<float>& heightData, int width, int height, float maxHeight) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Generate vertices
    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            float xPos = (float)x / (width - 1) - 0.5f;
            float zPos = (float)z / (height - 1) - 0.5f;
            float yPos = heightData[z * width + x] * maxHeight;

            vertices.push_back(xPos);  // X
            vertices.push_back(yPos); // Y
            vertices.push_back(zPos); // Z

            vertices.push_back((float)x / (width - 1)); // U
            vertices.push_back((float)z / (height - 1)); // V
        }
    }

    // Generate indices
    for (int z = 0; z < height - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            int topLeft = z * width + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * width + x;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    numIndices = (unsigned int)indices.size();

    // Create VAO, VBO, and EBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // TexCoords
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Heightmap::render() const {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
