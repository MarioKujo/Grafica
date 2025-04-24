// Este código es de dominio público
// andrmatgonros@gmail.com
#include "../Headers/Skybox.hpp"

namespace udit
{
    Skybox::Skybox()
    {
        setup_buffers();
    }

    Skybox::~Skybox()
    {
        // Liberación de recursos de OpenGL
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(1, &vbo_id);
        glDeleteTextures(1, &texture_id);
    }

    void Skybox::setup_buffers()
    {
        GLfloat vertices[] = {
            // Vértices de las seis caras del cubo del skybox
            -1.0f,  1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,  -1.0f, -1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,   1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f,   1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,   1.0f, -1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,   1.0f,  1.0f, -1.0f,   1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f
        };

        // Generación de buffers de OpenGL
        glGenVertexArrays(1, &vao_id);
        glGenBuffers(1, &vbo_id);

        glBindVertexArray(vao_id);

        // Configuración del buffer de vértices
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Configuración del atributo de vértices
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Skybox::set_texture(GLuint texture_id)
    {
        this->texture_id = texture_id;
    }

    GLuint Skybox::get_texture_id()
    {
        return texture_id;
    }

    void Skybox::render()
    {
        // Configuración para renderizar el skybox detrás de otros objetos
        glDepthFunc(GL_LEQUAL);

        // Renderizado del skybox
        glBindVertexArray(vao_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

        glDisable(GL_CULL_FACE);  // Desactiva la culling, ya que renderizamos todas las caras
        glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja los 36 vértices del cubo

        glBindVertexArray(0);

        // Restablece la función de depth testing
        glDepthFunc(GL_LESS);
    }
}
