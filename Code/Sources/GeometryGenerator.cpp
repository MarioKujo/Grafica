#include "../Headers/GeometryGenerator.hpp"
namespace udit
{
	MeshData GeometryGenerator::generatePlane(int hDiv, int vDiv, float width, float height) {
		MeshData data;
		int numVertices = (hDiv + 1) * (vDiv + 1);

		float dx = width / hDiv;
		float dz = height / vDiv;

		for (int i = 0; i <= vDiv; ++i) {
			for (int j = 0; j <= hDiv; ++j) {
				float x = j * dx - width / 2.0f;
				float z = i * dz - height / 2.0f;

				// Coordenadas
				data.coordinates.push_back(x);
				data.coordinates.push_back(0.0f);
				data.coordinates.push_back(z);

				// Normales (hacia arriba)
				data.normals.push_back(0.0f);
				data.normals.push_back(1.0f);
				data.normals.push_back(0.0f);

				// UV
				data.texCoords.push_back((float)j / hDiv);
				data.texCoords.push_back((float)i / vDiv);
			}
		}

		// Índices
		for (int i = 0; i < vDiv; ++i) {
			for (int j = 0; j < hDiv; ++j) {
				int row1 = i * (hDiv + 1);
				int row2 = (i + 1) * (hDiv + 1);

				int a = row1 + j;
				int b = row1 + j + 1;
				int c = row2 + j + 1;
				int d = row2 + j;

				data.indices.push_back(a);
				data.indices.push_back(b);
				data.indices.push_back(c);

				data.indices.push_back(c);
				data.indices.push_back(d);
				data.indices.push_back(a);
			}
		}

		return data;
	}

	MeshData GeometryGenerator::generateCone(float radius, float height, int sides) {
		MeshData data;

		// Centro de la base
		data.coordinates.insert(data.coordinates.end(), { 0.0f, 0.0f, 0.0f });
		data.normals.insert(data.normals.end(), { 0.0f, -1.0f, 0.0f });
		data.texCoords.insert(data.texCoords.end(), { 0.5f, 0.5f });

		// Círculo base + vértice superior
		for (int i = 0; i <= sides; ++i) {
			float angle = 2.0f * std::numbers::pi_v<float> *i / sides;
			float x = radius * cos(angle);
			float z = radius * sin(angle);

			// Base
			data.coordinates.insert(data.coordinates.end(), { x, 0.0f, z });
			data.normals.insert(data.normals.end(), { 0.0f, -1.0f, 0.0f });
			data.texCoords.insert(data.texCoords.end(), { (x / radius + 1.0f) * 0.5f, (z / radius + 1.0f) * 0.5f });
		}

		// Índices para la base
		for (int i = 1; i <= sides; ++i) {
			data.indices.insert(data.indices.end(), { 0, (unsigned int)i, (unsigned int)i + 1 });
		}

		int apexIndex = (int)data.coordinates.size() / 3;
		data.coordinates.insert(data.coordinates.end(), { 0.0f, height, 0.0f });
		data.normals.insert(data.normals.end(), { 0.0f, 1.0f, 0.0f });
		data.texCoords.insert(data.texCoords.end(), { 0.5f, 1.0f });

		// Laterales
		for (int i = 1; i <= sides; ++i) {
			int base1 = i;
			int base2 = i + 1;
			data.indices.insert(data.indices.end(), { (unsigned int)base1, (unsigned int)base2, (unsigned int)apexIndex });
		}

		return data;
	}

}