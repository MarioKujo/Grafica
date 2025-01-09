#include "../Headers/Cone.hpp"

namespace udit
{
    Cone::Cone(int d, GLfloat r, GLfloat h)
        : divisions(d), radius(r), height(h)
    {
        generateGeometry();

        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXCOORDS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Cone::~Cone()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Cone::generateGeometry()
    {
        int vertexCount = divisions + 2;  // Vértices para la base + vértice para el ápice
        int indexCount = divisions * 6;   // Índices para las caras laterales + la base

        coordinates.resize(vertexCount * 3);
        colors.resize(vertexCount * 3);
        texCoords.resize(vertexCount * 2);  // Coordenadas de textura (2D)
        indices.resize(indexCount);

        int vertexIndex = 0;

        // Coordenadas de los vértices de la base
        for (int i = 0; i < divisions; ++i)
        {
            GLfloat angle = (GLfloat)(2.0f * std::numbers::pi * i / divisions);
            coordinates[vertexIndex++] = radius * cos(angle);  // X
            coordinates[vertexIndex++] = 0.0f;                  // Y
            coordinates[vertexIndex++] = radius * sin(angle);  // Z

            // Coordenadas de textura para la base (un círculo)
            texCoords[vertexIndex / 3 * 2] = (GLfloat)(i) / divisions;  // U: Proporcional al ángulo
            texCoords[vertexIndex / 3 * 2 + 1] = 1.0f;                  // V: Para la base, siempre 1
        }

        // Vértice en el centro de la base
        int baseCenterIndex = vertexIndex / 3;
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = 0.0f;

        texCoords[baseCenterIndex * 2] = 0.5f;  // U (centro de la textura)
        texCoords[baseCenterIndex * 2 + 1] = 0.5f;  // V (centro de la textura)

        // Vértice en el ápice del cono
        int apexIndex = vertexIndex / 3;
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = height;
        coordinates[vertexIndex++] = 0.0f;

        texCoords[apexIndex * 2] = 0.5f;  // U (centrado en la cima)
        texCoords[apexIndex * 2 + 1] = 0.0f;  // V (en la cima)

        // Asignar colores a los vértices (opcional)
        GLfloat rainbowColors[7][3] = {
            {1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.29f, 0.0f, 0.51f}, {0.56f, 0.0f, 1.0f}
        };

        vertexIndex = 0;
        for (int i = 0; i < vertexCount; i++)
        {
            GLfloat* color = rainbowColors[i % 7];
            colors[vertexIndex++] = color[0];
            colors[vertexIndex++] = color[1];
            colors[vertexIndex++] = color[2];
        }

        // Índices para las caras laterales (conectar los vértices con el ápice)
        int index = 0;
        for (int i = 0; i < divisions; ++i)
        {
            int next = (i + 1) % divisions;

            // Coordenadas de textura para la cara lateral
            // U: Proporcional al ángulo de cada vértice
            texCoords[i * 2] = (GLfloat)(i) / divisions;

            // V: Interpolación lineal desde la base (V=1) hasta el ápice (V=0)
            texCoords[i * 2 + 1] = 1.0f - (i / (GLfloat)divisions);  // Ajuste para mantener las líneas horizontales

            texCoords[next * 2] = (GLfloat)(next) / divisions;  // U: Para el siguiente vértice
            texCoords[next * 2 + 1] = 1.0f - (next / (GLfloat)divisions);  // V: Interpolación lineal

            // Conectar los vértices de la cara lateral
            indices[index++] = i;         // Vértice de la base
            indices[index++] = apexIndex; // Vértice del ápice
            indices[index++] = next;      // Vértice de la base
        }

        // Índices para la base (conectar los vértices de la base entre sí)
        for (int i = 0; i < divisions; ++i)
        {
            int next = (i + 1) % divisions;
            indices[index++] = baseCenterIndex;  // Vértice del centro de la base
            indices[index++] = i;                // Vértice de la base
            indices[index++] = next;             // Vértice siguiente de la base
        }
    }


    void Cone::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, divisions * 6, GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
