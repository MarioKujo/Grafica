/*@file Cylinder.cpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Implementación de la clase Cylinder que representa un cilindro en un espacio 3D.
 *
 * Este archivo contiene la implementación de la clase Cylinder, que se encarga de generar
 * la geometría de un cilindro, la configuración de los buffers necesarios para su renderizado
 * utilizando OpenGL y su renderización en pantalla.
 */

#include "../Headers/Cylinder.hpp"

namespace udit
{
    /**
     * @brief Constructor de la clase Cylinder.
     *
     * Este constructor inicializa los parámetros del cilindro, como el número de
     * pilas (stack_count), el número de segmentos (slice_count), el radio y la altura,
     * y luego genera la geometría asociada a esos parámetros.
     * También crea los buffers necesarios para renderizar el cilindro en OpenGL.
     *
     * @param stack Número de divisiones verticales del cilindro (pilas).
     * @param slice Número de divisiones alrededor del eje del cilindro (segmentos).
     * @param r Radio de la base del cilindro.
     * @param h Altura del cilindro.
     */
    Cylinder::Cylinder(int stack, int slice, GLfloat r, GLfloat h)
        : stack_count(stack), slice_count(slice), radius(r), height(h)
    {
        generateGeometry();

        glGenBuffers(VBO_COUNT, vbo_ids);  ///< Genera los VBOs necesarios
        glGenVertexArrays(1, &vao_id); ///< Genera el VAO necesario

        glBindVertexArray(vao_id);

        // Crea y carga el VBO de coordenadas de vértices
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Crea y carga el VBO de coordenadas de textura
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXCOORDS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Crea y carga el EBO de índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    /**
     * @brief Destructor de la clase Cylinder.
     *
     * Este destructor se encarga de liberar los recursos utilizados por el cilindro,
     * como los buffers de OpenGL y el VAO.
     */
    Cylinder::~Cylinder()
    {
        glDeleteVertexArrays(1, &vao_id);  ///< Elimina el VAO
        glDeleteBuffers(VBO_COUNT, vbo_ids); ///< Elimina los VBOs
    }

    /**
     * @brief Genera la geometría del cilindro.
     *
     * Este método calcula los vértices, las coordenadas de textura y los índices
     * necesarios para representar el cilindro con la cantidad de pilas y segmentos
     * especificados.
     */
    void Cylinder::generateGeometry()
    {
        int vertexCount = stack_count * slice_count + 2; ///< Contador de vértices
        int indexCount = stack_count * slice_count * 6 + slice_count * 6; ///< Contador de índices

        coordinates.resize(vertexCount * 3);  ///< Reservar espacio para coordenadas de vértices
        texCoords.resize(vertexCount * 2);    ///< Reservar espacio para coordenadas de textura
        indices.resize(indexCount);           ///< Reservar espacio para índices

        int vertexIndex = 0;  ///< Índice para las coordenadas
        int texCoordIndex = 0;  ///< Índice para las coordenadas de textura
        for (int stack = 0; stack < stack_count; ++stack)
        {
            GLfloat y = -height / 2 + stack * (height / (stack_count - 1)); ///< Posición en el eje Y
            for (int slice = 0; slice < slice_count; ++slice)
            {
                GLfloat angle = static_cast<GLfloat>(2.0f * std::numbers::pi * slice / slice_count);  ///< Ángulo para la coordenada XZ
                GLfloat x = radius * cos(angle);  ///< Coordenada X
                GLfloat z = radius * sin(angle);  ///< Coordenada Z

                coordinates[vertexIndex++] = x;
                coordinates[vertexIndex++] = y;
                coordinates[vertexIndex++] = z;

                texCoords[texCoordIndex++] = static_cast<GLfloat>(stack) / stack_count;  ///< Coordenada de textura X
                texCoords[texCoordIndex++] = static_cast<GLfloat>(slice) / slice_count;  ///< Coordenada de textura Y
            }
        }

        // Vértices del centro de la base y la cima
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = -height / 2;
        coordinates[vertexIndex++] = 0.0f;

        texCoords[texCoordIndex++] = 0.5f;
        texCoords[texCoordIndex++] = 0.0f;

        int bottomCenterIndex = (vertexIndex / 3) - 1;

        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = height / 2;
        coordinates[vertexIndex++] = 0.0f;

        texCoords[texCoordIndex++] = 0.5f;
        texCoords[texCoordIndex++] = 1.0f;

        int topCenterIndex = (vertexIndex / 3) - 1;

        int index = 0;
        // Genera los índices para las caras laterales del cilindro
        for (int stack = 0; stack < stack_count - 1; ++stack)
        {
            for (int slice = 0; slice < slice_count; ++slice)
            {
                int nextSlice = (slice + 1) % slice_count;
                int current = stack * slice_count + slice;
                int next = stack * slice_count + nextSlice;
                int above = (stack + 1) * slice_count + slice;
                int aboveNext = (stack + 1) * slice_count + nextSlice;

                indices[index++] = current;
                indices[index++] = above;
                indices[index++] = next;

                indices[index++] = next;
                indices[index++] = above;
                indices[index++] = aboveNext;
            }
        }

        // Genera los índices para la base del cilindro
        for (int slice = 0; slice < slice_count; ++slice)
        {
            int nextSlice = (slice + 1) % slice_count;
            indices[index++] = bottomCenterIndex;
            indices[index++] = slice;
            indices[index++] = nextSlice;
        }

        // Genera los índices para la tapa superior del cilindro
        int offset = (stack_count - 1) * slice_count;
        for (int slice = 0; slice < slice_count; ++slice)
        {
            int nextSlice = (slice + 1) % slice_count;
            indices[index++] = topCenterIndex;
            indices[index++] = offset + nextSlice;
            indices[index++] = offset + slice;
        }
    }

    /**
     * @brief Renderiza el cilindro en la pantalla.
     *
     * Este método utiliza los buffers de OpenGL para dibujar el cilindro en la pantalla
     * usando las coordenadas de vértices, coordenadas de textura e índices previamente
     * generados.
     */
    void Cylinder::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  ///< Establece el modo de relleno para las caras
        glEnable(GL_CULL_FACE);  ///< Habilita la eliminación de caras traseras

        glBindVertexArray(vao_id);  ///< Activa el VAO
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_BYTE, 0);  ///< Dibuja los elementos utilizando los índices
        glBindVertexArray(0);  ///< Desactiva el VAO
    }
}
