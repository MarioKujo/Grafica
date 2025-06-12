
// Este código es de dominio público
// angel.rodriguez@udit.es

#include "../Headers/Cube.hpp"

namespace udit
{

    // Cada cara tiene 4 vértices únicos para permitir colores distintos por cara
    const GLfloat Cube::coordinates[] =
    {
        // Front face
        -1, -1, +1,
        +1, -1, +1,
        +1, +1, +1,
        -1, +1, +1,
        // Back face
        -1, -1, -1,
        +1, -1, -1,
        +1, +1, -1,
        -1, +1, -1,
        // Left face
        -1, -1, -1,
        -1, -1, +1,
        -1, +1, +1,
        -1, +1, -1,
        // Right face
        +1, -1, -1,
        +1, -1, +1,
        +1, +1, +1,
        +1, +1, -1,
        // Top face
        -1, +1, +1,
        +1, +1, +1,
        +1, +1, -1,
        -1, +1, -1,
        // Bottom face
        -1, -1, +1,
        +1, -1, +1,
        +1, -1, -1,
        -1, -1, -1,
    };

	const GLubyte Cube::indices[] =
	{
		// White (0, 1, 2, 3)
		0, 1, 2,
		0, 2, 3,

		// Yellow (4, 5, 6, 7)
		4, 6, 5,
		4, 7, 6,

		// Orange (8, 9, 10, 11)
		10, 8, 9,
		11, 8, 10,

		// Red (12, 13, 14, 15)
		12, 14, 13,
		12, 15, 14,

		// Green (16, 17, 18, 19)
		18, 16, 17,
		19, 16, 18,

		// Bottom face (20, 21, 22, 23)
		20, 22, 21,
		20, 23, 22
	};

    Cube::Cube()
    {
        // Se generan índices para los VBOs del cubo:

        glGenBuffers (VBO_COUNT, vbo_ids);
        glGenVertexArrays (1, &vao_id);

        // Se activa el VAO del cubo para configurarlo:

        glBindVertexArray (vao_id);

        // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);

        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un VBO los datos de color y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray (1);
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un EBO los datos de índices:

        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray (0);
    }

    Cube::~Cube()
    {
        // Se liberan los VBOs y el VAO usados:

        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers      (VBO_COUNT, vbo_ids);
    }

    void Cube::render(const std::array<glm::vec3, 6>& face_colors)
    {
        // Construir el array de colores para cada vértice (4 por cara)
        GLfloat face_vertex_colors[24 * 3]; // 24 vértices, 3 componentes por vértice

        for (int i = 0; i < 6; ++i) {
            glm::vec3 color = face_colors[i];
            for (int j = 0; j < 4; ++j) {
                face_vertex_colors[(i * 4 + j) * 3 + 0] = color.r;
                face_vertex_colors[(i * 4 + j) * 3 + 1] = color.g;
                face_vertex_colors[(i * 4 + j) * 3 + 2] = color.b;
            }
        }

        // Subir al VBO de color en cada render
        glBindVertexArray(vao_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(face_vertex_colors), face_vertex_colors, GL_DYNAMIC_DRAW);
        glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_BYTE, 0); // 6 caras × 2 triángulos × 3 vértices
        glBindVertexArray(0);
    }
}
