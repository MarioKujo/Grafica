/*@file Heightmap.cpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Implementación de la clase Heightmap para manejar mapas de altura en la generación de terrenos.
 *
 * Esta implementación se encarga de cargar un mapa de altura desde una imagen en escala de grises,
 * generar una malla de vértices correspondiente al terreno, y renderizarla utilizando OpenGL.
 * La clase utiliza la librería stb_image para cargar imágenes y OpenGL para la visualización.
 */

#include "../Headers/Heightmap.hpp"
#include "../Headers/stb_image.hpp"
#include <iostream>

 /**
  * @brief Constructor de la clase Heightmap.
  *
  * Carga los datos del mapa de altura desde una imagen y genera la malla
  * correspondiente utilizando esos datos.
  *
  * @param heightmapPath Ruta al archivo de la imagen del mapa de altura.
  * @param width Ancho del mapa de altura.
  * @param height Altura del mapa de altura.
  * @param maxHeight Altura máxima que pueden alcanzar los vértices.
  */
Heightmap::Heightmap(const std::string& heightmapPath, float width, float height, float maxHeight)
    : vao(0), vbo(0), ebo(0), textureID(0), numIndices(0) {
    int imgWidth, imgHeight;
    // Carga los datos de altura desde la imagen y genera la malla correspondiente.
    auto heightData = loadHeightData(heightmapPath, imgWidth, imgHeight);
    if (!heightData.empty()) {
        generateMesh(heightData, imgWidth, imgHeight, maxHeight);
    }
}

/**
 * @brief Destructor de la clase Heightmap.
 *
 * Libera los recursos de OpenGL asociados con el mapa de altura.
 */
Heightmap::~Heightmap() {
    // Libera los recursos de OpenGL si están inicializados.
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
}

/**
 * @brief Carga los datos de altura desde un archivo de imagen.
 *
 * Lee una imagen de mapa de altura en escala de grises, normaliza los valores
 * y devuelve un vector con los datos de altura.
 *
 * @param heightmapPath Ruta al archivo de la imagen del mapa de altura.
 * @param width Ancho de la imagen cargada.
 * @param height Altura de la imagen cargada.
 * @return std::vector<float> Vector con los datos de altura normalizados.
 */
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

    // Libera la memoria de la imagen después de su uso.
    stbi_image_free(data);
    return heightData;
}

/**
 * @brief Genera la malla de vértices e índices para el mapa de altura.
 *
 * Genera la malla del terreno utilizando los datos de altura, y calcula
 * las posiciones y coordenadas de textura correspondientes para los vértices.
 *
 * @param heightData Datos de altura del mapa.
 * @param width Ancho del mapa de altura.
 * @param height Altura del mapa de altura.
 * @param maxHeight Altura máxima de los vértices en la malla.
 */
void Heightmap::generateMesh(const std::vector<float>& heightData, int width, int height, float maxHeight) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Genera los vértices con las posiciones y coordenadas de textura.
    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            float xPos = (float)x / (width - 1) - 0.5f;  // Normaliza las coordenadas X
            float zPos = (float)z / (height - 1) - 0.5f;  // Normaliza las coordenadas Z
            float yPos = heightData[z * width + x] * maxHeight;  // Aplica la altura máxima

            // Añade la posición del vértice y las coordenadas de textura (UV)
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);

            vertices.push_back((float)x / (width - 1));  // Coordenada de textura X
            vertices.push_back((float)z / (height - 1)); // Coordenada de textura Y
        }
    }

    // Genera los índices de los triángulos que forman la malla.
    for (int z = 0; z < height - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            int topLeft = z * width + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * width + x;
            int bottomRight = bottomLeft + 1;

            // Añade los índices para los triángulos de la malla
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    numIndices = (unsigned int)indices.size();

    // Genera los objetos de OpenGL para los buffers y el VAO.
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Asocia el VAO y los VBO/EBOM con OpenGL.
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Establece los atributos de los vértices: posiciones y coordenadas de textura.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

/**
 * @brief Renderiza el mapa de altura.
 *
 * Dibuja la malla utilizando los vértices e índices generados en OpenGL.
 */
void Heightmap::render() const {
    // Renderiza el mapa de altura usando los buffers de OpenGL.
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
