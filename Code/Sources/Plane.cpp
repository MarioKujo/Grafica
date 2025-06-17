#include "../Headers/Plane.hpp"

namespace udit
{
    // Inicialización de punteros estáticos
    GLfloat* Plane::coordinates = nullptr;
    GLfloat* Plane::colors = nullptr;
    GLubyte* Plane::indices = nullptr;

    /**
     * @brief Constructor de la clase Plane.
     *
     * Genera la geometría del plano, inicializa los VBOs y VAO, y configura los datos de los buffers.
     */
    Plane::Plane(int width, int height) : grid_width(width), grid_height(height)
    {
        // Generar la geometría
        generateGeometry();

        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        // VBO para coordenadas
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, grid_width * grid_height * 3 * sizeof(GLfloat),
            coordinates, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // VBO para colores
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, grid_width * grid_height * 3 * sizeof(GLfloat),
            colors, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // EBO para índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            (grid_width - 1) * (grid_height - 1) * 6 * sizeof(GLubyte), indices,
            GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    /**
     * @brief Destructor de la clase Plane.
     *
     * Libera los recursos utilizados, incluyendo los punteros dinámicos, VBOs y el VAO.
     */
    Plane::~Plane()
    {
        // Liberar los recursos
        delete[] coordinates;
        delete[] colors;
        delete[] indices;

        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    /**
     * @brief Genera la geometría del plano.
     *
     * Calcula las coordenadas de los vértices, los colores y los índices para los triángulos
     * que forman la cuadrícula del plano.
     */
    void Plane::generateGeometry()
    {
        int vertexCount = grid_width * grid_height; // Número total de vértices
        int indexCount = (grid_width - 1) * (grid_height - 1) * 6; // Índices para los triángulos

        // Reservar memoria inicializando con valores por defecto
        coordinates = new GLfloat[vertexCount * 3]();
        colors = new GLfloat[vertexCount * 3]();
        indices = new GLubyte[indexCount]();

        // Generar coordenadas
        int vertexIndex = 0;
        for (int y = 0; y < grid_height; ++y)
        {
            for (int x = 0; x < grid_width; ++x)
            {
                GLfloat posX = x * 2.0f - (grid_width - 1);  // X
                GLfloat posZ = y * 2.0f - (grid_height - 1); // Z
                if (vertexIndex + 3 <= vertexCount * 3) // Validar límites
                {
                    coordinates[vertexIndex++] = posX;
                    coordinates[vertexIndex++] = 0.0f;
                    coordinates[vertexIndex++] = posZ; // Z siempre 0 para un plano
                }
            }
        }

        // Generar colores
        vertexIndex = 0;
        for (int i = 0; i < vertexCount; ++i)
        {
            if (vertexIndex + 3 <= vertexCount * 3) // Validar límites
            {
                colors[vertexIndex++] = 0.0f; // R
                colors[vertexIndex++] = 1.0f; // G
                colors[vertexIndex++] = 0.0f; // B (Verde)
            }
        }

        // Generar índices
        int index = 0;
        for (int y = 0; y < grid_height - 1; ++y)
        {
            for (int x = 0; x < grid_width - 1; ++x)
            {
                int topLeft = y * grid_width + x;
                int topRight = topLeft + 1;
                int bottomLeft = (y + 1) * grid_width + x;
                int bottomRight = bottomLeft + 1;

                if (index + 6 <= indexCount) // Validar límites
                {
                    // Primer triángulo
                    indices[index++] = topLeft;
                    indices[index++] = bottomLeft;
                    indices[index++] = bottomRight;

                    // Segundo triángulo
                    indices[index++] = topLeft;
                    indices[index++] = bottomRight;
                    indices[index++] = topRight;
                }
            }
        }
    }


    /**
     * @brief Renderiza el plano.
     *
     * Dibuja el plano utilizando el modo de líneas y desactiva el culling para mostrar ambas caras.
     */
    void Plane::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, (grid_width - 1) * (grid_height - 1) * 6, GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
