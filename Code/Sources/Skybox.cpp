#include "../Headers/Skybox.hpp"

namespace udit
{

    Skybox::Skybox(MeshData m, ShaderProgram* s):mesh(m), shader(s), textureID(0)
    {}

    Skybox::~Skybox()
    {
        glDeleteTextures(1, &textureID);
    }

    void Skybox::setTexture(GLuint textureID)
    {
        this->textureID = textureID;
    }

    GLuint Skybox::getTextureID()
    {
        return textureID;
    }

    void Skybox::render(const glm::mat4& view, const glm::mat4& projection)
    {
        shader->use();
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        glDisable(GL_CULL_FACE);
        mesh.render();

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }
}
