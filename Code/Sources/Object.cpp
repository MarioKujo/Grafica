#include "../Headers/Object.hpp"

namespace udit
{
    Object::Object(Mesh* m, ShaderProgram* s)
        : mesh(m), shader(s), position(0), rotation(0), scale(1), textureID(0), heightmapID(0)
    {
        heightmapLoc = glGetUniformLocation(shader->id(), "heightmap");
        transparencyLoc = glGetUniformLocation(shader->id(), "transparency");
    }

    void Object::render(const glm::mat4& view, const glm::mat4& projection)
    {
        shader->use();
        glActiveTexture(GL_TEXTURE0);
        if (heightmapLoc != -1)
        {
            shader->setInt("heightmap", 0);
            glBindTexture(GL_TEXTURE_2D, heightmapID);
            glActiveTexture(GL_TEXTURE1);
            shader->setInt("textureSampler", 1);
            glBindTexture(GL_TEXTURE_2D, textureID);

            shader->setFloat("height_scale", 25.f);
        }
        else if (transparencyLoc != -1)
        {
            shader->setInt("textureSampler", 0);

            glBindTexture(GL_TEXTURE_2D, textureID);

            glDepthMask(GL_FALSE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            shader->setFloat("transparency", 0.5f);
        }
        else
        {
            shader->setInt("textureSampler", 0);
            glBindTexture(GL_TEXTURE_2D, textureID);
        }

        glm::mat4 model = computeModelMatrix();
        shader->setMat4("model_view_matrix", (view * model));
        shader->setMat4("projection_matrix", projection);
        mesh->render();

        if (transparencyLoc != -1)
        {
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
        }
    }

    void Object::setPosition(const glm::vec3& pos) { position = pos; }
    void Object::setRotation(const glm::vec3& rot) { rotation = rot; }
    void Object::setScale(const glm::vec3& scl) { scale = scl; }

    void Object::setTextureID(GLuint texture)
    {
        textureID = texture;
    }

    void Object::setHeightmapTextureID(GLuint texture)
    {
        heightmapID = texture;
    }

    ShaderProgram* Object::getShader() const
    {
        return shader;
    }

    Mesh* Object::getMesh() const
    {
        return mesh;
    }

    glm::mat4 Object::computeModelMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        model = glm::scale(model, scale);
        return model;
    }
}
