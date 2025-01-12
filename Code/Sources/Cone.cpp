/*@file Cone.cpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Implementación de la clase Cone que representa un cono 3D.
 *
 * Este archivo contiene las definiciones de los métodos de la clase Cone que se utilizan
 * para generar la geometría del cono, configurar los buffers de OpenGL, y renderizarlo
 * en la pantalla.
 */

#include "../Headers/Cone.hpp"

namespace udit
{
    /**
     * @brief Constructor de la clase Cone.
     *
     * Este constructor inicializa un cono con un número determinado de divisiones,
     * radio y altura. Además, genera la geometría del cono y configura los buffers
     * necesarios para su renderizado en OpenGL.
     *
     * @param d Número de divisiones del cono.
     * @param r Radio de la base del cono.
     * @param h Altura del cono.
     */
    Cone::Cone(int d, GLfloat r, GLfloat h)
        : divisions(d), radius(r), height(h)
    {
        generateGeometry();

        // Generación de buffers
        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        // Configuración del VBO para las coordenadas de los vértices
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Configuración del VBO para las coordenadas de textura
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXCOORDS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Configuración del EBO para los índices de los vértices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    /**
     * @brief Destructor de la clase Cone.
     *
     * Este destructor se encarga de liberar los recursos de OpenGL utilizados
     * por el cono, como el Vertex Array Object (VAO) y los Vertex Buffer Objects (VBOs).
     */
    Cone::~Cone()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    /**
     * @brief Genera la geometría del cono.
     *
     * Este método calcula las coordenadas de los vértices, las coordenadas de textura
     * y los índices que definen la geometría del cono. Se utiliza para crear el modelo
     * del cono en el espacio 3D.
     */
    void Cone::generateGeometry()
    {
        int vertexCount = divisions + 2;  /**< Número total de vértices (base + vértice superior) */
        int indexCount = divisions * 6;   /**< Número total de índices (para los triángulos) */

        coordinates.resize(vertexCount * 3);  /**< Vértices del cono (3 coordenadas por vértice) */
        texCoords.resize(vertexCount * 2);   /**< Coordenadas de textura (2 coordenadas por vértice) */
        indices.resize(indexCount);          /**< Índices para los triángulos */

        int vertexIndex = 0;

        // Generación de vértices para la base del cono
        for (int i = 0; i < divisions; ++i)
        {
            GLfloat angle = (GLfloat)(2.0f * std::numbers::pi * i / divisions);
            coordinates[vertexIndex++] = radius * cos(angle);
            coordinates[vertexIndex++] = 0.0f;
            coordinates[vertexIndex++] = radius * sin(angle);

            texCoords[vertexIndex / 3 * 2] = (GLfloat)(i) / divisions;
            texCoords[vertexIndex / 3 * 2 + 1] = 1.0f;
        }

        // Vértice central de la base
        int baseCenterIndex = vertexIndex / 3;
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = 0.0f;

        texCoords[baseCenterIndex * 2] = 0.5f;
        texCoords[baseCenterIndex * 2 + 1] = 0.5f;

        // Vértice superior del cono (ápice)
        int apexIndex = vertexIndex / 3;
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = height;
        coordinates[vertexIndex++] = 0.0f;

        texCoords[apexIndex * 2] = 0.5f;
        texCoords[apexIndex * 2 + 1] = 0.0f;

        // Generación de índices para las caras laterales
        int index = 0;
        for (int i = 0; i < divisions; ++i)
        {
            int next = (i + 1) % divisions;

            texCoords[i * 2] = (GLfloat)(i) / divisions;
            texCoords[i * 2 + 1] = 1.0f;

            texCoords[next * 2] = (GLfloat)(next) / divisions;
            texCoords[next * 2 + 1] = 1.0f;

            indices[index++] = i;
            indices[index++] = apexIndex;
            indices[index++] = next;
        }

        // Generación de índices para la base
        for (int i = 0; i < divisions; ++i)
        {
            int next = (i + 1) % divisions;
            indices[index++] = baseCenterIndex;
            indices[index++] = i;
            indices[index++] = next;
        }
    }

    /**
     * @brief Renderiza el cono en la pantalla.
     *
     * Este método utiliza los buffers de OpenGL para dibujar el cono con los datos
     * generados previamente. Se establece el modo de dibujo de triángulos y se activa
     * el culling para optimizar el renderizado.
     */
    void Cone::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  /**< Establece el modo de relleno para los polígonos. */
        glEnable(GL_CULL_FACE);  /**< Activa el culling para eliminar caras traseras. */

        glBindVertexArray(vao_id);  /**< Activa el VAO del cono. */
        glDrawElements(GL_TRIANGLES, divisions * 6, GL_UNSIGNED_BYTE, 0);  /**< Dibuja los triángulos del cono. */
        glBindVertexArray(0);  /**< Desactiva el VAO. */
    }
}
