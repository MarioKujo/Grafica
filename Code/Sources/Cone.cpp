// Este código es de dominio público
// andrmatgonros@gmail.com
#include "../Headers/Cone.hpp"

namespace udit
{
	Cone::Cone(int d, GLfloat r, GLfloat h)
		: divisions(d), radius(r), height(h)
	{
		generateGeometry();
		generateBuffers();
	}

	Cone::~Cone()
	{
		glDeleteVertexArrays(1, &vao_id);
		glDeleteBuffers(VBO_COUNT, vbo_ids);
	}

	void Cone::generateGeometry()
	{
		const int vertexCount = divisions + 2;  // Vértices: base + ápice
		const int indexCount = divisions * 6;   // Índices para triángulos

		coordinates.resize(vertexCount * 3);  // 3 coordenadas por vértice
		texCoords.resize(vertexCount * 2);   // 2 coordenadas de textura por vértice
		normals.resize(vertexCount * 3);      // 3 componentes por normal (x, y, z)
		indices.resize(indexCount);          // Índices para triángulos

		int vertexIndex = 0;

		// Generar vértices de la base
		for (int i = 0; i < divisions; ++i)
		{
			GLfloat angle = static_cast<GLfloat>(2.0f * std::numbers::pi * i / divisions);
			coordinates[vertexIndex++] = radius * cos(angle);
			coordinates[vertexIndex++] = 0.0f;
			coordinates[vertexIndex++] = radius * sin(angle);

			// Coordenadas de textura
			int texIndex = (vertexIndex / 3 - 1) * 2;
			texCoords[texIndex] = static_cast<GLfloat>(i) / divisions;
			texCoords[texIndex + 1] = 1.0f;

			// Normal para la base (todas son (0, -1, 0))
			normals[vertexIndex - 3] = 0.0f;
			normals[vertexIndex - 2] = -1.0f;  // Normal hacia abajo
			normals[vertexIndex - 1] = 0.0f;
		}

		// Coordenadas del centro de la base
		int baseCenterIndex = vertexIndex / 3;
		coordinates[vertexIndex++] = 0.0f;
		coordinates[vertexIndex++] = 0.0f;
		coordinates[vertexIndex++] = 0.0f;

		texCoords[baseCenterIndex * 2] = 0.5f;
		texCoords[baseCenterIndex * 2 + 1] = 0.5f;

		// Normal para el centro de la base (también (0, -1, 0))
		normals[baseCenterIndex * 3] = 0.0f;
		normals[baseCenterIndex * 3 + 1] = -1.0f;
		normals[baseCenterIndex * 3 + 2] = 0.0f;

		// Coordenadas del ápice
		int apexIndex = vertexIndex / 3;
		coordinates[vertexIndex++] = 0.0f;
		coordinates[vertexIndex++] = height;
		coordinates[vertexIndex++] = 0.0f;

		texCoords[apexIndex * 2] = 0.5f;
		texCoords[apexIndex * 2 + 1] = 0.0f;

		// Normal para el ápice (por ahora podemos usar un vector nulo)
		normals[apexIndex * 3] = 0.0f;
		normals[apexIndex * 3 + 1] = 1.0f;
		normals[apexIndex * 3 + 2] = 0.0f;

		// Generar índices para las caras laterales
		int index = 0;
		for (int i = 0; i < divisions; ++i)
		{
			int next = (i + 1) % divisions;

			// Índices para las caras laterales
			indices[index++] = i;
			indices[index++] = apexIndex;
			indices[index++] = next;

			// Calcular la normal para la cara lateral
			GLfloat x1 = coordinates[3 * i] - coordinates[3 * apexIndex];
			GLfloat y1 = coordinates[3 * i + 1] - coordinates[3 * apexIndex + 1];
			GLfloat z1 = coordinates[3 * i + 2] - coordinates[3 * apexIndex + 2];

			GLfloat x2 = coordinates[3 * next] - coordinates[3 * apexIndex];
			GLfloat y2 = coordinates[3 * next + 1] - coordinates[3 * apexIndex + 1];
			GLfloat z2 = coordinates[3 * next + 2] - coordinates[3 * apexIndex + 2];

			// Producto cruzado para obtener la normal
			GLfloat nx = y1 * z2 - z1 * y2;
			GLfloat ny = z1 * x2 - x1 * z2;
			GLfloat nz = x1 * y2 - y1 * x2;

			// Normalizar la normal
			GLfloat length = sqrt(nx * nx + ny * ny + nz * nz);
			nx /= length;
			ny /= length;
			nz /= length;

			// Asignar la normal a los tres vértices
			normals[3 * i] = nx;
			normals[3 * i + 1] = ny;
			normals[3 * i + 2] = nz;

			normals[3 * apexIndex] = nx;
			normals[3 * apexIndex + 1] = ny;
			normals[3 * apexIndex + 2] = nz;

			normals[3 * next] = nx;
			normals[3 * next + 1] = ny;
			normals[3 * next + 2] = nz;
		}

		// Generar índices para la base
		for (int i = 0; i < divisions; ++i)
		{
			int next = (i + 1) % divisions;
			indices[index++] = baseCenterIndex;
			indices[index++] = i;
			indices[index++] = next;
		}
	}
}
