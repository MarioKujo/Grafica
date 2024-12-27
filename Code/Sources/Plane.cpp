#include "../Headers/Plane.hpp"

namespace udit
{
    GLfloat* Plane::coordinates = nullptr;
    GLfloat* Plane::colors = nullptr;
    GLubyte* Plane::indices = nullptr;

    Plane::Plane()
    {
        // Generar la geometría
        generateGeometry();

        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        // VBO para coordenadas
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, GRID_WIDTH * GRID_HEIGHT * 3 * sizeof(GLfloat), coordinates, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // VBO para colores
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, GRID_WIDTH * GRID_HEIGHT * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // EBO para índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GRID_WIDTH - 1) * (GRID_HEIGHT - 1) * 6 * sizeof(GLubyte), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Plane::~Plane()
    {
        // Liberar los recursos
        delete[] coordinates;
        delete[] colors;
        delete[] indices;

        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Plane::generateGeometry()
    {
        int vertexCount = GRID_WIDTH * GRID_HEIGHT;
        int indexCount = (GRID_WIDTH - 1) * (GRID_HEIGHT - 1) * 6;

        // Reservar memoria para las coordenadas, colores e índices
        coordinates = new GLfloat[vertexCount * 3];
        colors = new GLfloat[vertexCount * 3];
        indices = new GLubyte[indexCount];

        // Generar coordenadas
        int vertexIndex = 0;
        for (int y = 0; y < GRID_HEIGHT; ++y)
        {
            for (int x = 0; x < GRID_WIDTH; ++x)
            {
                GLfloat posX = x * 2.0f - (GRID_WIDTH - 1);  // Ajuste para centrar en el eje X
                GLfloat posY = y * 2.0f - (GRID_HEIGHT - 1); // Ajuste para centrar en el eje Y
                coordinates[vertexIndex++] = posX;
                coordinates[vertexIndex++] = posY;
                coordinates[vertexIndex++] = 0.0f;  // Z siempre 0 para un plano
            }
        }

        // Generar colores
        vertexIndex = 0;
        for (int i = 0; i < vertexCount; ++i)
        {
            colors[vertexIndex++] = 0.0f;  // R
            colors[vertexIndex++] = 1.0f;  // G
            colors[vertexIndex++] = 0.0f;  // B (Verde)
        }

        // Generar índices
        int index = 0;
        for (int y = 0; y < GRID_HEIGHT - 1; ++y)
        {
            for (int x = 0; x < GRID_WIDTH - 1; ++x)
            {
                int topLeft = y * GRID_WIDTH + x;
                int topRight = topLeft + 1;
                int bottomLeft = (y + 1) * GRID_WIDTH + x;
                int bottomRight = bottomLeft + 1;

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

    void Plane::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, (GRID_WIDTH - 1) * (GRID_HEIGHT - 1) * 6, GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
