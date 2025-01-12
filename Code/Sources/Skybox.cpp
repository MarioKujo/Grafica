/**
 * @file Skybox.cpp
 * @author andrmatgonros@gmail.com
 * @date
 *
 * Este archivo contiene la implementación de la clase Skybox, la cual se encarga de manejar
 * un skybox en una escena 3D. El skybox se utiliza para crear un fondo panorámico estático
 * que envuelve toda la escena, generalmente representando el cielo o el horizonte.
 */

#include "../Headers/Skybox.hpp"

 /**
  * @brief Constructor de la clase Skybox.
  *
  * Este constructor recibe un vector de rutas de texturas que se usarán para las seis caras
  * del cubo del skybox. Además, inicializa los buffers necesarios para renderizar el skybox.
  *
  * @param faces Vector que contiene las rutas a las texturas de las seis caras del cubo del skybox.
  */
Skybox::Skybox(const std::vector<std::string>& faces)
    : faces(faces) {
    setup_buffers();
}

/**
 * @brief Destructor de la clase Skybox.
 *
 * El destructor elimina los recursos utilizados por el skybox, como los buffers de OpenGL
 * (VAO, VBO) y la textura.
 */
Skybox::~Skybox() {
    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
    glDeleteTextures(1, &texture_id);
}

/**
 * @brief Configura los buffers necesarios para renderizar el skybox.
 *
 * Este método crea y configura los buffers de OpenGL (VAO, VBO) que almacenan los vértices
 * necesarios para representar las seis caras del cubo del skybox.
 *
 * La geometría del cubo está definida por un conjunto de vértices que corresponden a las
 * posiciones de las caras del cubo.
 */
void Skybox::setup_buffers() {
    GLfloat vertices[] = {
        // Vértices de las seis caras del cubo del skybox
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // Generación de buffers de OpenGL
    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &vbo_id);

    glBindVertexArray(vao_id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configuración del atributo de vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * @brief Establece la textura para el skybox.
 *
 * Este método asigna un identificador de textura al skybox. La textura será utilizada
 * para mapear las caras del cubo.
 *
 * @param texture_id El identificador de la textura para el skybox.
 */
void Skybox::set_texture(GLuint texture_id) {
    this->texture_id = texture_id;
}

/**
 * @brief Obtiene el identificador de la textura del skybox.
 *
 * Este método retorna el identificador de la textura que se ha asignado al skybox.
 *
 * @return GLuint El identificador de la textura del skybox.
 */
GLuint Skybox::get_texture_id() {
    return texture_id;
}

/**
 * @brief Renderiza el skybox en la escena.
 *
 * Este método renderiza el cubo del skybox, usando la textura asignada para las caras del cubo.
 * La función se asegura de que el skybox siempre se dibuje detrás de los demás objetos en la
 * escena, utilizando la configuración adecuada para el depth testing.
 */
void Skybox::render() {
    // Configuración para asegurarse de que el skybox esté detrás de los objetos
    glDepthFunc(GL_LEQUAL);

    glBindVertexArray(vao_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    glDisable(GL_CULL_FACE);  // Desactiva la culling, ya que queremos renderizar todas las caras
    glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibuja los 36 vértices del cubo

    glBindVertexArray(0);
    glDepthFunc(GL_LESS);  // Restablece la función de depth testing
}
