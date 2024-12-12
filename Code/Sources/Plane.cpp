#include "../Headers/Plane.hpp"

namespace udit
{
    const GLfloat Plane::coordinates[] =
    {
        -3.0f, -2.0f, 0.0f,   // 0
        -2.0f, -2.0f, 0.0f,   // 1
        -2.0f,  2.0f, 0.0f,   // 2
        -3.0f,  2.0f, 0.0f,   // 3
         3.0f,  2.0f, 0.0f,   // 4
         3.0f, -2.0f, 0.0f,   // 5
         2.0f, -2.0f, 0.0f,   // 6
         2.0f,  2.0f, 0.0f,   // 7
        -1.0f, -2.0f, 0.0f,   // 8
         0.0f, -2.0f, 0.0f,   // 9
         0.0f,  2.0f, 0.0f,   // 10
        -1.0f,  2.0f, 0.0f,   // 11
         1.0f,  2.0f, 0.0f,   // 12
         1.0f, -2.0f, 0.0f,   // 13
    };


    const GLfloat Plane::colors[] =
    {
        0.0f, 1.0f, 0.0f,   // Verde
        0.0f, 1.0f, 0.0f,   // Verde
        0.0f, 1.0f, 0.0f,   // Verde
        0.0f, 1.0f, 0.0f,   // Verde
        0.0f, 1.0f, 0.0f,   // Verde
        0.0f, 1.0f, 0.0f,   // Verde
        0.0f, 1.0f, 0.0f,   // Verde
        0.0f, 1.0f, 0.0f,   // Verde
        0.0f, 1.0f, 0.0f,   // Verde
        0.0f, 1.0f, 0.0f,   // Verde
        0.0f, 1.0f, 0.0f,   // Verde
        0.0f, 1.0f, 0.0f,   // Verde
    };

    const GLubyte Plane::indices[] =
    {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        1, 2, 11, 1, 8, 11,
    };

    Plane::Plane()
    {
        // Se generan índices para los VBOs del plano:

        glGenBuffers (VBO_COUNT, vbo_ids);
        glGenVertexArrays (1, &vao_id);

        // Se activa el VAO del plano para configurarlo:

        glBindVertexArray (vao_id);

        // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);

        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un VBO los datos de color y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        glEnableVertexAttribArray (1);
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un EBO los datos de índices:

        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray (0);
    }

    Plane::~Plane()
    {
        // Se liberan los VBOs y el VAO usados:

        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers      (VBO_COUNT, vbo_ids);
    }

    void Plane::render()
    {
        // Se selecciona el VAO que contiene los datos del objeto y se dibujan sus elementos:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glBindVertexArray (vao_id);
        glDrawElements    (GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray (0);
    }
}
