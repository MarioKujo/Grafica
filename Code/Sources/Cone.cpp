/* @file Cone.cpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 */
#include "../Headers/Cone.hpp"

namespace udit
{
    Cone::Cone(int d, GLfloat r, GLfloat h)
        : divisions(d), radius(r), height(h)
    {
        generateGeometry();

        // Generar y configurar buffers de OpenGL
        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        // Configuración de las coordenadas de vértices
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Configuración de coordenadas de textura
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXCOORDS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Configuración de índices para elementos
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0); // Desvincular VAO
    }

    Cone::~Cone()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Cone::generateGeometry()
    {
        const int vertexCount = divisions + 2;  // Vértices: base + ápice
        const int indexCount = divisions * 6;   // Índices para triángulos

        coordinates.resize(vertexCount * 3);  // 3 coordenadas por vértice
        texCoords.resize(vertexCount * 2);   // 2 coordenadas de textura por vértice
        indices.resize(indexCount);          // Índices para triángulos

        int vertexIndex = 0;

        // Generar vértices de la base
        for (int i = 0; i < divisions; ++i)
        {
            GLfloat angle = static_cast<GLfloat>(2.0f * std::numbers::pi * i / divisions);
            coordinates[vertexIndex++] = radius * cos(angle);
            coordinates[vertexIndex++] = 0.0f;
            coordinates[vertexIndex++] = radius * sin(angle);

            int texIndex = (vertexIndex / 3 - 1) * 2;
            texCoords[texIndex] = static_cast<GLfloat>(i) / divisions;
            texCoords[texIndex + 1] = 1.0f;
        }

        // Coordenadas del centro de la base
        int baseCenterIndex = vertexIndex / 3;
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = 0.0f;

        texCoords[baseCenterIndex * 2] = 0.5f;
        texCoords[baseCenterIndex * 2 + 1] = 0.5f;

        // Coordenadas del ápice
        int apexIndex = vertexIndex / 3;
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = height;
        coordinates[vertexIndex++] = 0.0f;

        texCoords[apexIndex * 2] = 0.5f;
        texCoords[apexIndex * 2 + 1] = 0.0f;

        // Generar índices para las caras laterales
        int index = 0;
        for (int i = 0; i < divisions; ++i)
        {
            int next = (i + 1) % divisions;
            indices[index++] = i;
            indices[index++] = apexIndex;
            indices[index++] = next;
        }

        // Generar índices para la base
        for (int i = 0; i < divisions; ++i)
        {
            int next = (i + 1) % divisions;
            indices[index++] = baseCenterIndex;
            indices[index++] = i;
            indices[index++] = next;
        }
    }

    void Cone::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, divisions * 6, GL_UNSIGNED_BYTE, nullptr);
        glBindVertexArray(0);
    }
}
