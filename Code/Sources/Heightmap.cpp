/* @file Heightmap.cpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 */
#include "../Headers/Heightmap.hpp"
#include "../Headers/stb_image.hpp"
#include <iostream>

namespace udit {

    // Constructor de la clase Heightmap.
    // Carga los datos del mapa de altura desde una imagen y genera la malla correspondiente.
    Heightmap::Heightmap(const std::string& heightmapPath, float width, float height, float maxHeight)
        : vao(0), vbo(0), ebo(0), textureID(0), numIndices(0) {

        int imgWidth, imgHeight;
        auto heightData = loadHeightData(heightmapPath, imgWidth, imgHeight);

        if (!heightData.empty()) {
            generateMesh(heightData, imgWidth, imgHeight, maxHeight);
        }
    }

    // Destructor de la clase Heightmap.
    // Libera los recursos de OpenGL asociados al mapa de altura.
    Heightmap::~Heightmap() {
        if (vao) glDeleteVertexArrays(1, &vao);
        if (vbo) glDeleteBuffers(1, &vbo);
        if (ebo) glDeleteBuffers(1, &ebo);
    }

    // Carga los datos de altura desde un archivo de imagen (escala de grises).
    // Normaliza los valores de altura entre 0 y 1 y devuelve un vector con los datos.
    std::vector<float> Heightmap::loadHeightData(const std::string& heightmapPath, int& width, int& height) {
        int channels;
        unsigned char* data = stbi_load(heightmapPath.c_str(), &width, &height, &channels, 1);

        if (!data) {
            std::cerr << "Failed to load heightmap: " << heightmapPath << std::endl;
            return {};
        }

        std::vector<float> heightData(width * height);

        // Normaliza los valores de altura entre 0 y 1.
        for (int i = 0; i < width * height; ++i) {
            heightData[i] = data[i] / 255.0f;
        }

        stbi_image_free(data);
        return heightData;
    }

    // Genera la malla de vértices e índices para el mapa de altura.
    // Calcula las posiciones, coordenadas de textura y genera los índices de los triángulos.
    void Heightmap::generateMesh(const std::vector<float>& heightData, int width, int height, float maxHeight) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Genera vértices con posiciones y coordenadas de textura.
        for (int z = 0; z < height; ++z) {
            for (int x = 0; x < width; ++x) {
                float xPos = (float)x / (width - 1) - 0.5f;  // Normaliza X
                float zPos = (float)z / (height - 1) - 0.5f; // Normaliza Z
                float yPos = heightData[z * width + x] * maxHeight; // Altura

                // Añade posición y coordenadas de textura.
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(zPos);

                vertices.push_back((float)x / (width - 1));  // UV X
                vertices.push_back((float)z / (height - 1)); // UV Y
            }
        }

        // Genera índices para los triángulos de la malla.
        for (int z = 0; z < height - 1; ++z) {
            for (int x = 0; x < width - 1; ++x) {
                int topLeft = z * width + x;
                int topRight = topLeft + 1;
                int bottomLeft = (z + 1) * width + x;
                int bottomRight = bottomLeft + 1;

                // Añade índices para los triángulos.
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        numIndices = static_cast<unsigned int>(indices.size());

        // Genera los buffers de OpenGL para la malla.
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        // Carga los datos de los vértices en el buffer.
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Carga los índices de los triángulos.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Configura los atributos de los vértices (posición y textura).
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    // Renderiza el mapa de altura.
    // Dibuja la malla utilizando los vértices e índices generados.
    void Heightmap::render() const {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
