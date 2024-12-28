#include "../Headers/Cylinder.hpp"

namespace udit
{

    GLfloat* Cylinder::coordinates = nullptr; /**< Coordenadas de los vértices del cilindro. */
    GLfloat* Cylinder::colors = nullptr; /**< Colores de los vértices del cilindro. */
    GLubyte* Cylinder::indices = nullptr; /**< Índices de los vértices para triángulos. */

    /**
     * @brief Constructor de la clase Cylinder.
     *
     * Inicializa los recursos de OpenGL y genera la geometría del cilindro.
     */
    Cylinder::Cylinder(int stack, int slice, GLfloat r, GLfloat h)
    {
        stack_count = stack;
        slice_count = slice;
        radius = r;
        height = h;
        generateGeometry();

        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        // VBO para coordenadas
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, stack_count * slice_count * 3 * sizeof(GLfloat)
            + 6 * 3 * sizeof(GLfloat), coordinates, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // VBO para colores
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, slice_count * stack_count * 3 * sizeof(GLfloat),
            colors, GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // EBO para índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, stack_count * slice_count * 6 * sizeof(GLubyte)
            + slice_count * 6, indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    /**
     * @brief Destructor de la clase Cylinder.
     *
     * Libera los recursos de memoria dinámica y de OpenGL utilizados.
     */
    Cylinder::~Cylinder()
    {
        delete[] coordinates;
        delete[] colors;
        delete[] indices;

        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    /**
     * @brief Genera la geometría del cilindro.
     *
     * Calcula las coordenadas de los vértices, los colores de cada vértice y los índices.
     */
    void Cylinder::generateGeometry()
    {
        int vertexCount = stack_count * slice_count + 2; // Vértices del cilindro (+centros tapas).
        int indexCount = stack_count * slice_count * 6 + slice_count * 6; // Índices para triángulos.

        // Asignar memoria
        coordinates = new GLfloat[vertexCount * 3]();
        colors = new GLfloat[vertexCount * 3]();
        indices = new GLubyte[indexCount]();

        // Generar vértices
        int vertexIndex = 0;
        for (int stack = 0; stack < stack_count; ++stack)
        {
            GLfloat y = -height / 2 + stack * (height / (stack_count - 1));
            for (int slice = 0; slice < slice_count; ++slice)
            {
                GLfloat angle = (GLfloat)(2.0f * std::numbers::pi * slice / slice_count);
                GLfloat x = radius * cos(angle);
                GLfloat z = radius * sin(angle);

                if (vertexIndex + 3 <= vertexCount * 3) // Validar límites
                {
                    coordinates[vertexIndex] = x;
                    coordinates[vertexIndex + 1] = y;
                    coordinates[vertexIndex + 2] = z;
                    vertexIndex += 3;
                }
            }
        }

        // Vértices centrales para las tapas
        if (vertexIndex + 3 <= vertexCount * 3) // Validar límites
        {
            coordinates[vertexIndex] = 0.0f;
            coordinates[vertexIndex + 1] = -height / 2;
            coordinates[vertexIndex + 2] = 0.0f;
        }
        int bottomCenterIndex = vertexIndex / 3;
        vertexIndex += 3;

        if (vertexIndex + 3 <= vertexCount * 3) // Validar límites
        {
            coordinates[vertexIndex] = 0.0f;
            coordinates[vertexIndex + 1] = height / 2;
            coordinates[vertexIndex + 2] = 0.0f;
        }
        int topCenterIndex = vertexIndex / 3;

        // Generar colores de arcoíris
        GLfloat rainbowColors[7][3] = {
            {1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.29f, 0.0f, 0.51f}, {0.56f, 0.0f, 1.0f} };

        vertexIndex = 0;
        for (int stack = 0; stack < stack_count; ++stack)
        {
            for (int slice = 0; slice < slice_count; ++slice)
            {
                GLfloat* color = rainbowColors[slice % 7];
                if (vertexIndex + 3 <= vertexCount * 3) // Validar límites
                {
                    colors[vertexIndex] = color[0];
                    colors[vertexIndex + 1] = color[1];
                    colors[vertexIndex + 2] = color[2];
                    vertexIndex += 3;
                }
            }
        }

        // Generar índices
        int index = 0;
        for (int stack = 0; stack < stack_count - 1; ++stack)
        {
            for (int slice = 0; slice < slice_count; ++slice)
            {
                int nextSlice = (slice + 1) % slice_count;
                int current = stack * slice_count + slice;
                int next = stack * slice_count + nextSlice;
                int above = (stack + 1) * slice_count + slice;
                int aboveNext = (stack + 1) * slice_count + nextSlice;

                if (index + 6 <= indexCount) // Validar límites
                {
                    indices[index++] = current;
                    indices[index++] = next;
                    indices[index++] = above;

                    indices[index++] = next;
                    indices[index++] = aboveNext;
                    indices[index++] = above;
                }
            }
        }

        // Índices para las tapas
        for (int slice = 0; slice < slice_count; ++slice)
        {
            int nextSlice = (slice + 1) % slice_count;

            if (index + 3 <= indexCount) // Validar límites
            {
                indices[index++] = bottomCenterIndex;
                indices[index++] = slice;
                indices[index++] = nextSlice;
            }
        }

        int offset = (stack_count - 1) * slice_count;
        for (int slice = 0; slice < slice_count; ++slice)
        {
            int nextSlice = (slice + 1) % slice_count;

            if (index + 3 <= indexCount) // Validar límites
            {
                indices[index++] = topCenterIndex;
                indices[index++] = offset + slice;
                indices[index++] = offset + nextSlice;
            }
        }
    }


    /**
     * @brief Renderiza el cilindro.
     *
     * Configura el modo de renderizado y dibuja los elementos.
     */
    void Cylinder::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, stack_count * slice_count * 6 + slice_count * 6,
            GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
