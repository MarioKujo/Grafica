#include "../Headers/Mesh.hpp"

namespace udit
{
    Mesh::Mesh() : isInitialized(false)
    {
        // Crear buffers vacíos para la malla
        generateBuffers();
    }

    Mesh::Mesh(MeshData receivedData) : data(std::move(receivedData)), isInitialized(false)
    {
        // Crear buffers con los datos recibidos
        generateBuffers();
    }

    void Mesh::render()
    {
        if (!isInitialized) return;

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);

        // Dibujar la malla usando el VAO
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(data.indices.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void Mesh::generateBuffers()
    {
        // Generar VAO y VBOs (posición, texcoords, normales, índices)
        glGenVertexArrays(1, &vao_id);
        glGenBuffers(VBO_COUNT, vbo_ids);

        glBindVertexArray(vao_id);

        // Configurar buffer para las posiciones
        uploadBuffer(vbo_ids[COORDINATES_VBO], GL_ARRAY_BUFFER, data.coordinates.data(), data.coordinates.size() * sizeof(GLfloat));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Configurar buffer para las coordenadas de textura
        uploadBuffer(vbo_ids[TEXCOORDS_VBO], GL_ARRAY_BUFFER, data.texCoords.data(), data.texCoords.size() * sizeof(GLfloat));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Configurar buffer para las normales
        uploadBuffer(vbo_ids[NORMALS_VBO], GL_ARRAY_BUFFER, data.normals.data(), data.normals.size() * sizeof(GLfloat));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Configurar buffer para los índices (EBO)
        uploadBuffer(vbo_ids[INDICES_EBO], GL_ELEMENT_ARRAY_BUFFER, data.indices.data(), data.indices.size() * sizeof(GLuint));

        glBindVertexArray(0);
        isInitialized = true;
    }

    void Mesh::uploadBuffer(GLuint bufferId, GLenum target, const void* dataPtr, size_t dataSize)
    {
        // Función auxiliar para cargar datos en buffers OpenGL
        glBindBuffer(target, bufferId);
        glBufferData(target, dataSize, dataPtr, GL_STATIC_DRAW);
    }

    void Mesh::deleteBuffers()
    {
        if (!isInitialized) return;

        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);

        isInitialized = false;
    }
}
