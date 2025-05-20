#include "../Headers/Skybox.hpp"

namespace udit
{
    // Constructor que inicializa la malla, el shader y establece textureID a 0
    Skybox::Skybox(MeshData m, ShaderProgram* s) : mesh(m), shader(s), textureID(0)
    {}

    // Destructor que libera la textura del skybox
    Skybox::~Skybox()
    {
        glDeleteTextures(1, &textureID);
    }

    // Establece el ID de la textura del skybox
    void Skybox::setTexture(GLuint textureID)
    {
        this->textureID = textureID;
    }

    // Retorna el ID de la textura del skybox
    GLuint Skybox::getTextureID()
    {
        return textureID;
    }

    // Renderiza el skybox usando las matrices de vista y proyección
    void Skybox::render(const glm::mat4& view, const glm::mat4& projection)
    {
        shader->use();

        // Quitar la traslación de la matriz de vista para que el skybox siga la cámara
        glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));
        shader->setMat4("view", viewNoTranslation);
        shader->setMat4("projection", projection);

        // Configurar el buffer de profundidad para que el skybox se dibuje correctamente
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        // Bind de la textura del cubemap
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        // Deshabilitar el face culling para que se vean todas las caras
        glDisable(GL_CULL_FACE);

        // Dibujar la malla del skybox
        mesh.render();

        // Restaurar el estado del buffer de profundidad y face culling
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
    }
}
