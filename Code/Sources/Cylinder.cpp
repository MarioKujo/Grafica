#include "../Headers/Cylinder.hpp"

namespace udit
{
    const int Cylinder::STACK_COUNT = 10;
    const int Cylinder::SLICE_COUNT = 10;
    const GLfloat Cylinder::RADIUS = 2.0f;
    const GLfloat Cylinder::HEIGHT = 5.0f;

    GLfloat* Cylinder::coordinates = nullptr;
    GLfloat* Cylinder::colors = nullptr;
    GLubyte* Cylinder::indices = nullptr;

    Cylinder::Cylinder()
    {
        generateGeometry();

        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        // VBO para coordenadas
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, STACK_COUNT * SLICE_COUNT * 3 * sizeof(GLfloat) + 6 * 3 * sizeof(GLfloat), coordinates, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // VBO para colores
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, SLICE_COUNT * STACK_COUNT * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // EBO para índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, STACK_COUNT * SLICE_COUNT * 6 * sizeof(GLubyte) + SLICE_COUNT * 6, indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Cylinder::~Cylinder()
    {
        delete[] coordinates;
        delete[] colors;
        delete[] indices;

        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Cylinder::generateGeometry()
    {
        int vertexCount = STACK_COUNT * SLICE_COUNT + 2; // +2 para los centros de las caras
        int indexCount = STACK_COUNT * SLICE_COUNT * 6 + SLICE_COUNT * 6;

        coordinates = new GLfloat[vertexCount * 3];
        colors = new GLfloat[vertexCount * 3];
        indices = new GLubyte[indexCount];

        // Generar vértices
        int vertexIndex = 0;
        for (int stack = 0; stack < STACK_COUNT; ++stack)
        {
            GLfloat y = -HEIGHT / 2 + stack * (HEIGHT / (STACK_COUNT - 1));
            for (int slice = 0; slice < SLICE_COUNT; ++slice)
            {
                GLfloat angle = (GLfloat)(2.0f * std::numbers::pi * slice / SLICE_COUNT);
                GLfloat x = RADIUS * cos(angle);
                GLfloat z = RADIUS * sin(angle);

                coordinates[vertexIndex] = x;
                coordinates[vertexIndex + 1] = y;
                coordinates[vertexIndex + 2] = z;

                vertexIndex += 3;
            }
        }

        // Vértices centrales para las tapas
        coordinates[vertexIndex] = 0.0f;
        coordinates[vertexIndex + 1] = -HEIGHT / 2;
        coordinates[vertexIndex + 2] = 0.0f;
        int bottomCenterIndex = vertexIndex / 3;
        vertexIndex += 3;

        coordinates[vertexIndex] = 0.0f;
        coordinates[vertexIndex + 1] = HEIGHT / 2;
        coordinates[vertexIndex + 2] = 0.0f;
        int topCenterIndex = vertexIndex / 3;

        // Colores del arcoíris para cada arista vertical
        GLfloat rainbowColors[7][3] = {
            {1.0f, 0.0f, 0.0f},  // Rojo
            {1.0f, 0.5f, 0.0f},  // Naranja
            {1.0f, 1.0f, 0.0f},  // Amarillo
            {0.0f, 1.0f, 0.0f},  // Verde
            {0.0f, 0.0f, 1.0f},  // Azul
            {0.29f, 0.0f, 0.51f},// Añil
            {0.56f, 0.0f, 1.0f}  // Violeta
        };

        vertexIndex = 0;
        for (int stack = 0; stack < STACK_COUNT; ++stack)
        {
            for (int slice = 0; slice < SLICE_COUNT; ++slice)
            {
                // Elegir un color basado en la arista vertical
                GLfloat* color = rainbowColors[slice % 7];
                colors[vertexIndex] = color[0];
                colors[vertexIndex + 1] = color[1];
                colors[vertexIndex + 2] = color[2];

                vertexIndex += 3;
            }
        }

        // Generar índices
        int index = 0;
        for (int stack = 0; stack < STACK_COUNT - 1; ++stack)
        {
            for (int slice = 0; slice < SLICE_COUNT; ++slice)
            {
                int nextSlice = (slice + 1) % SLICE_COUNT;
                int current = stack * SLICE_COUNT + slice;
                int next = stack * SLICE_COUNT + nextSlice;
                int above = (stack + 1) * SLICE_COUNT + slice;
                int aboveNext = (stack + 1) * SLICE_COUNT + nextSlice;

                indices[index++] = current;
                indices[index++] = next;
                indices[index++] = above;

                indices[index++] = next;
                indices[index++] = aboveNext;
                indices[index++] = above;
            }
        }

        // Índices para la tapa inferior
        for (int slice = 0; slice < SLICE_COUNT; ++slice)
        {
            int nextSlice = (slice + 1) % SLICE_COUNT;
            indices[index++] = bottomCenterIndex;
            indices[index++] = slice;
            indices[index++] = nextSlice;
        }

        // Índices para la tapa superior
        int offset = (STACK_COUNT - 1) * SLICE_COUNT;
        for (int slice = 0; slice < SLICE_COUNT; ++slice)
        {
            int nextSlice = (slice + 1) % SLICE_COUNT;
            indices[index++] = topCenterIndex;
            indices[index++] = offset + slice;
            indices[index++] = offset + nextSlice;
        }
    }


    void Cylinder::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, STACK_COUNT * SLICE_COUNT * 6 + SLICE_COUNT * 6, GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
