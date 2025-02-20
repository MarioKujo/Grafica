#pragma once
#include <glad/glad.h>
#include <vector>
using namespace std;
namespace udit
{
	class Mesh
	{
	public:
		Mesh();
		void render();
		void generateBuffers();
	protected:
		GLuint vao_id;               ///< Identificador del Vertex Array Object (VAO).
		GLuint vbo_ids[3];           ///< Identificadores de los Vertex Buffer Objects (VBOs) para las coordenadas, coordenadas de textura e índices.

		enum { COORDINATES_VBO, TEXCOORDS_VBO, INDICES_EBO, VBO_COUNT }; ///< Índices para los VBOs y el EBO.

		vector<GLfloat> coordinates; ///< Vectores que contienen las coordenadas de los vértices del plano.
		vector<GLfloat> texCoords;   ///< Vectores que contienen las coordenadas de textura (UV) para los vértices.
		vector<GLubyte> indices;     ///< Vectores que contienen los índices para dibujar el plano con triángulos.
	};
}
