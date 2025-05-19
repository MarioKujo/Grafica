#pragma once
#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <numbers>
#include "GeometryGenerator.hpp"
#include "ShaderProgram.hpp"
using namespace std;
namespace udit
{
	class Mesh
	{
	public:
		Mesh();
		Mesh(MeshData receivedData);
		void render();
		void generateBuffers();
		void deleteBuffers();
	protected:

		enum { COORDINATES_VBO, TEXCOORDS_VBO, NORMALS_VBO, INDICES_EBO, VBO_COUNT };

		bool isInitialized;

		GLuint vao_id;
		GLuint vbo_ids[VBO_COUNT];

		MeshData data;
	};
}
