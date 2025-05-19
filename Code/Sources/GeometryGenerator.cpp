#include "../Headers/GeometryGenerator.hpp"

namespace udit
{
    // Función auxiliar para agregar un vértice con sus datos a MeshData
    static void addVertex(MeshData& data, const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& uv) {
        data.coordinates.insert(data.coordinates.end(), { pos.x, pos.y, pos.z });
        data.normals.insert(data.normals.end(), { normal.x, normal.y, normal.z });
        data.texCoords.insert(data.texCoords.end(), { uv.x, uv.y });
    }

    // Genera un plano subdividido en hDiv x vDiv con dimensiones width x height
    MeshData GeometryGenerator::generatePlane(int hDiv, int vDiv, float width, float height) {
        MeshData data;

        float dx = width / hDiv;
        float dz = height / vDiv;
        glm::vec3 normal(0.0f, 1.0f, 0.0f);

        // Generar vértices, normales y UVs
        for (int i = 0; i <= vDiv; ++i) {
            for (int j = 0; j <= hDiv; ++j) {
                glm::vec3 pos(j * dx - width / 2.0f, 0.0f, i * dz - height / 2.0f);
                glm::vec2 uv((float)j / hDiv, (float)i / vDiv);

                addVertex(data, pos, normal, uv);
            }
        }

        // Crear índices para triángulos
        for (int i = 0; i < vDiv; ++i) {
            for (int j = 0; j < hDiv; ++j) {
                int row1 = i * (hDiv + 1);
                int row2 = (i + 1) * (hDiv + 1);

                int a = row1 + j;
                int b = row1 + j + 1;
                int c = row2 + j + 1;
                int d = row2 + j;

                data.indices.insert(data.indices.end(), { (unsigned int)a, (unsigned int)b, (unsigned int)c, (unsigned int)c, (unsigned int)d, (unsigned int)a });
            }
        }

        return data;
    }

    // Genera un cono con base en el origen y altura en Y positiva
    MeshData GeometryGenerator::generateCone(float radius, float height, int sides) {
        MeshData data;

        // Centro de la base
        addVertex(data, { 0.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.5f, 0.5f });

        // Base
        for (int i = 0; i <= sides; ++i) {
            float angle = 2.0f * std::numbers::pi_v<float> *i / sides;
            glm::vec3 pos(radius * cos(angle), 0.0f, radius * sin(angle));
            glm::vec2 uv((pos.x / radius + 1.0f) * 0.5f, (pos.z / radius + 1.0f) * 0.5f);

            addVertex(data, pos, { 0.0f, -1.0f, 0.0f }, uv);
        }

        // Índices base (triángulos tipo pizza)
        for (int i = 1; i <= sides; ++i) {
            data.indices.insert(data.indices.end(), { 0, (unsigned int)i, (unsigned int)(i + 1) });
        }

        // Ápice del cono
        int apexIndex = static_cast<int>(data.coordinates.size() / 3);
        addVertex(data, { 0.0f, height, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.5f, 1.0f });

        // Caras laterales
        for (int i = 1; i <= sides; ++i) {
            data.indices.insert(data.indices.end(), { (unsigned int)i, (unsigned int)(i + 1), (unsigned int)apexIndex });
        }

        return data;
    }

    // Genera un cubo centrado en el origen con lados de tamaño `size`
    MeshData GeometryGenerator::generateCube(float size) {
        MeshData data;
        float s = size / 2.0f;

        struct Face {
            glm::vec3 normal;
            glm::vec3 vertices[4];
            glm::vec2 uvs[4];
        };

        Face faces[6] = {
            {{ 0,  0,  1}, {{-s, -s,  s}, { s, -s,  s}, { s,  s,  s}, {-s,  s,  s}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}}},
            {{ 0,  0, -1}, {{ s, -s, -s}, {-s, -s, -s}, {-s,  s, -s}, { s,  s, -s}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}}},
            {{ 1,  0,  0}, {{ s, -s,  s}, { s, -s, -s}, { s,  s, -s}, { s,  s,  s}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}}},
            {{-1,  0,  0}, {{-s, -s, -s}, {-s, -s,  s}, {-s,  s,  s}, {-s,  s, -s}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}}},
            {{ 0,  1,  0}, {{-s,  s,  s}, { s,  s,  s}, { s,  s, -s}, {-s,  s, -s}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}}},
            {{ 0, -1,  0}, {{-s, -s, -s}, { s, -s, -s}, { s, -s,  s}, {-s, -s,  s}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}}},
        };

        for (const auto& face : faces) {
            GLuint startIdx = static_cast<GLuint>(data.coordinates.size() / 3);

            for (int i = 0; i < 4; ++i) {
                addVertex(data, face.vertices[i], face.normal, face.uvs[i]);
            }

            data.indices.insert(data.indices.end(), {
                startIdx, startIdx + 1, startIdx + 2,
                startIdx, startIdx + 2, startIdx + 3
                });
        }

        return data;
    }
}
