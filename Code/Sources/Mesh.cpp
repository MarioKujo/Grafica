#include "../Headers/Mesh.hpp"
namespace udit
{
    Mesh::Mesh() : isInitialized(false)
	{
        generateBuffers();
	}
    Mesh::Mesh(const MeshData receivedData) : data(receivedData)
    {
        generateBuffers();
    }
    void Mesh::render()
    {
        if (!isInitialized)
        {
            return;
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glDisable(GL_CULL_FACE);
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, (GLsizei)data.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    void Mesh::generateBuffers()
    {
        // Generar VBOs y VAO
        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        // Configuración del VAO
        glBindVertexArray(vao_id);

        // Configuración del VBO para las coordenadas de los vértices
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, data.coordinates.size() * sizeof(GLfloat), data.coordinates.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Configuración del VBO para las coordenadas de textura
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXCOORDS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, data.texCoords.size() * sizeof(GLfloat), data.texCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Configuración del VBO para las normales
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, data.normals.size() * sizeof(GLfloat), data.normals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Configuración del EBO para los índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(GLuint), data.indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
        isInitialized = true;
    }
    void Mesh::deleteBuffers()
    {
        if (!isInitialized) return;

        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
        isInitialized = false;
    }
}