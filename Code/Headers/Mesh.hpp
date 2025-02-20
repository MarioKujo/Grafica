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

		enum { COORDINATES_VBO, TEXCOORDS_VBO, NORMALS_VBO, INDICES_EBO, VBO_COUNT }; ///< Índices para los VBOs y el EBO.

		GLuint vao_id;               ///< Identificador del Vertex Array Object (VAO).
		GLuint vbo_ids[VBO_COUNT];           ///< Identificadores de los Vertex Buffer Objects (VBOs) para las coordenadas, coordenadas de textura e índices.

		vector<GLfloat> coordinates; ///< Vectores que contienen las coordenadas de los vértices del plano.
		vector<GLfloat> texCoords;   ///< Vectores que contienen las coordenadas de textura (UV) para los vértices.
		vector<GLfloat> normals;     ///< Vectores que contienen las normales para la iluminación
		vector<GLubyte> indices;     ///< Vectores que contienen los índices para dibujar el plano con triángulos.
	};
}
