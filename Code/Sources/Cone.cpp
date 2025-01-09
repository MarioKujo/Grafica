#include "../Headers/Cone.hpp"

namespace udit
{

    GLfloat* Cone::coordinates = nullptr;
    GLfloat* Cone::colors = nullptr;
    GLubyte* Cone::indices = nullptr;

    Cone::Cone(int d, GLfloat r, GLfloat h)
        : divisions(d), radius(r), height(h)
    {
        generateGeometry();

        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        // VBO para coordenadas
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, (divisions + 2) * 3 * sizeof(GLfloat), coordinates, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // VBO para colores
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, (divisions + 2) * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // EBO para índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, divisions * 6 * sizeof(GLubyte), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Cone::~Cone()
    {
        delete[] coordinates;
        delete[] colors;
        delete[] indices;

        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Cone::generateGeometry()
    {
        int vertexCount = divisions + 2; // Vértices: base + centro base + vértice superior
        int indexCount = divisions * 6; // Triángulos: base y lados

        // Asignar memoria
        coordinates = new GLfloat[vertexCount * 3]();
        colors = new GLfloat[vertexCount * 3]();
        indices = new GLubyte[indexCount]();

        int vertexIndex = 0;

        // Generar coordenadas de la base
        for (int i = 0; i < divisions; ++i)
        {
            GLfloat angle = (GLfloat)(2.0f * std::numbers::pi * i / divisions);

            if (vertexIndex + 3 <= vertexCount * 3) {
                coordinates[vertexIndex] = radius * cos(angle);
                coordinates[vertexIndex + 1] = 0.0f; // Base en y = 0
                coordinates[vertexIndex + 2] = radius * sin(angle);
                vertexIndex += 3;
            }
        }

        // Coordenadas del centro de la base
        int baseCenterIndex = vertexIndex / 3;
        if (vertexIndex + 3 <= vertexCount * 3) {
            coordinates[vertexIndex] = 0.0f;
            coordinates[vertexIndex + 1] = 0.0f;
            coordinates[vertexIndex + 2] = 0.0f;
            vertexIndex += 3;
        }

        // Coordenadas del vértice superior
        int apexIndex = vertexIndex / 3;
        if (vertexIndex + 3 <= vertexCount * 3) {
            coordinates[vertexIndex] = 0.0f;
            coordinates[vertexIndex + 1] = height;
            coordinates[vertexIndex + 2] = 0.0f;
            vertexIndex += 3;
        }
        // Generar colores de arcoíris
        GLfloat rainbowColors[7][3] = {
            {1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.29f, 0.0f, 0.51f}, {0.56f, 0.0f, 1.0f} };
        // Generar colores
        vertexIndex = 0;
        for (int i = 0; i < vertexCount; i ++)
        {
            GLfloat* color = rainbowColors[i % 7];
            colors[vertexIndex++] = color[0];
            colors[vertexIndex++] = color[1];
            colors[vertexIndex++] = color[2];
        }

        // Generar índices
        int index = 0;
        for (int i = 0; i < divisions; ++i)
        {
            int next = (i + 1) % divisions;

            // Triángulos laterales
            if (index + 3 <= indexCount) { // Validar límite
                indices[index] = i;
                indices[index + 1] = next;
                indices[index + 2] = apexIndex;
                index += 3;
            }

            // Triángulos de la base
            if (index + 3 <= indexCount) { // Validar límite
                indices[index] = baseCenterIndex;
                indices[index + 1] = next;
                indices[index + 2] = i;
                index += 3;
            }
        }
    }


    void Cone::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, divisions * 6, GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
