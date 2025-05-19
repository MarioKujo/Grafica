#pragma once
#include <vector>
#include <glm.hpp>
#include <glad/glad.h>
#include <numbers>

namespace udit {
    struct MeshData {
        std::vector<GLfloat> coordinates;
        std::vector<GLfloat> texCoords;
        std::vector<GLfloat> normals;
        std::vector<GLuint> indices;
    };

    class GeometryGenerator {
    public:
        static MeshData generatePlane(int hDivisions, int vDivisions, float width, float height);
        static MeshData generateCone(float radius, float height, int segments);
        static MeshData generateCube(float size = 1.0f);

    };
}
