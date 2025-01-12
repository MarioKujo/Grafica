/*@file Plane.cpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Implementación de la clase Plane para representar un plano 3D.
 *
 * Esta clase genera un plano de malla utilizando OpenGL. El plano tiene una
 * cuadrícula definida por su ancho y alto, y se utiliza para representar superficies planas.
 */

#include "../Headers/Plane.hpp"
#include <vector>
using namespace std;

namespace udit
{
    /**
     * @brief Constructor de la clase Plane.
     *
     * Inicializa un plano con las dimensiones de la cuadrícula especificadas (ancho y alto).
     * Genera la geometría del plano y configura los buffers de OpenGL (VBO, VAO, EBO) para renderizarlo.
     *
     * @param width El ancho del plano, define la cantidad de columnas de la cuadrícula.
     * @param height La altura del plano, define la cantidad de filas de la cuadrícula.
     */
    Plane::Plane(int width, int height) : grid_width(width), grid_height(height)
    {
        generateGeometry();

        // Generar los VBOs y VAO
        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        // Configurar el VAO
        glBindVertexArray(vao_id);

        // Configurar el VBO para las coordenadas de los vértices
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Configurar el VBO para las coordenadas de textura
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXCOORDS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Configurar el EBO para los índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    /**
     * @brief Destructor de la clase Plane.
     *
     * Libera los recursos de OpenGL utilizados para almacenar la geometría del plano.
     */
    Plane::~Plane()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    /**
     * @brief Genera la geometría del plano.
     *
     * Calcula las posiciones de los vértices, las coordenadas de textura y los índices
     * que definen la malla del plano en función de las dimensiones de la cuadrícula.
     */
    void Plane::generateGeometry()
    {
        int vertexCount = grid_width * grid_height;
        int indexCount = (grid_width - 1) * (grid_height - 1) * 6;

        coordinates.resize(vertexCount * 3, 0.0f);
        texCoords.resize(vertexCount * 2, 0.0f);
        indices.resize(indexCount, 0);

        int vertexIndex = 0;
        int texCoordIndex = 0;

        // Generar las posiciones y las coordenadas de textura para cada vértice
        for (int y = 0; y < grid_height; ++y)
        {
            for (int x = 0; x < grid_width; ++x)
            {
                GLfloat posX = x * 2.0f - (grid_width - 1);
                GLfloat posY = y * 2.0f - (grid_height - 1);

                coordinates[vertexIndex++] = posX;
                coordinates[vertexIndex++] = posY;
                coordinates[vertexIndex++] = 0.0f;

                texCoords[texCoordIndex++] = static_cast<GLfloat>(x) / (grid_width - 1);
                texCoords[texCoordIndex++] = static_cast<GLfloat>(y) / (grid_height - 1);
            }
        }

        // Generar los índices para los triángulos de la malla
        int index = 0;
        for (int y = 0; y < grid_height - 1; ++y)
        {
            for (int x = 0; x < grid_width - 1; ++x)
            {
                int topLeft = y * grid_width + x;
                int topRight = topLeft + 1;
                int bottomLeft = (y + 1) * grid_width + x;
                int bottomRight = bottomLeft + 1;

                // Dos triángulos por cuadrícula
                indices[index++] = topLeft;
                indices[index++] = bottomLeft;
                indices[index++] = bottomRight;
                indices[index++] = topLeft;
                indices[index++] = bottomRight;
                indices[index++] = topRight;
            }
        }
    }

    /**
     * @brief Renderiza el plano.
     *
     * Configura OpenGL para dibujar el plano utilizando los buffers de geometría generados.
     */
    void Plane::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glDisable(GL_CULL_FACE);
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
