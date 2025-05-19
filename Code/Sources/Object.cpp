#include "../Headers/Object.hpp"

namespace udit
{
    Object::Object(Mesh* m, ShaderProgram* s)
        : mesh(m), shader(s), position(0), rotation(0), scale(1), textureID(0), heightmapID(0)
    {
        // Obtener ubicaciones de uniform variables importantes en el shader
        heightmapLoc = glGetUniformLocation(shader->id(), "heightmap");
        transparencyLoc = glGetUniformLocation(shader->id(), "transparency");
    }

    void Object::render(const glm::mat4& view, const glm::mat4& projection)
    {
        shader->use();  // Activar shader
        setupTexturesAndBlending();  // Configurar texturas y blending según sea necesario

        // Calcular y enviar las matrices modelo-vista y proyección al shader
        glm::mat4 model = computeModelMatrix();
        shader->setMat4("model_view_matrix", view * model);
        shader->setMat4("projection_matrix", projection);

        mesh->render();  // Dibujar la malla

        cleanupBlending();  // Restaurar estado de blending si fue modificado
    }

    void Object::setupTexturesAndBlending()
    {
        glActiveTexture(GL_TEXTURE0);  // Activar la unidad de textura 0

        if (heightmapLoc != -1)
        {
            bindHeightmapAndTexture();  // Vincular heightmap y textura principal
        }
        else if (transparencyLoc != -1)
        {
            bindTextureWithTransparency();  // Vincular textura con blending para transparencia
        }
        else
        {
            bindTextureOnly();  // Vincular solo la textura principal
        }
    }

    void Object::bindHeightmapAndTexture()
    {
        shader->setInt("heightmap", 0);  // Establecer el uniform para heightmap
        glBindTexture(GL_TEXTURE_2D, heightmapID);  // Vincular la textura del heightmap

        glActiveTexture(GL_TEXTURE1);  // Activar unidad de textura 1
        shader->setInt("textureSampler", 1);  // Establecer uniform para textura principal
        glBindTexture(GL_TEXTURE_2D, textureID);  // Vincular la textura principal

        shader->setFloat("height_scale", 25.f);  // Establecer escala para heightmap
    }

    void Object::bindTextureWithTransparency()
    {
        shader->setInt("textureSampler", 0);  // Establecer uniform para textura principal
        glBindTexture(GL_TEXTURE_2D, textureID);  // Vincular la textura principal

        glDepthMask(GL_FALSE);  // Deshabilitar escritura en el buffer de profundidad
        glEnable(GL_BLEND);  // Habilitar blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Configurar función de blending

        shader->setFloat("transparency", 0.5f);  // Establecer nivel de transparencia
    }

    void Object::bindTextureOnly()
    {
        shader->setInt("textureSampler", 0);  // Establecer uniform para textura principal
        glBindTexture(GL_TEXTURE_2D, textureID);  // Vincular la textura principal
    }

    void Object::cleanupBlending()
    {
        if (transparencyLoc != -1)
        {
            glDisable(GL_BLEND);  // Deshabilitar blending si se habilitó antes
            glDepthMask(GL_TRUE);  // Habilitar escritura en el buffer de profundidad
        }
    }

    // Setters
    void Object::setPosition(const glm::vec3& pos) { position = pos; }
    void Object::setRotation(const glm::vec3& rot) { rotation = rot; }
    void Object::setScale(const glm::vec3& scl) { scale = scl; }
    void Object::setTextureID(GLuint texture) { textureID = texture; }
    void Object::setHeightmapTextureID(GLuint texture) { heightmapID = texture; }

    // Getters
    ShaderProgram* Object::getShader() const { return shader; }
    Mesh* Object::getMesh() const { return mesh; }

    glm::mat4 Object::computeModelMatrix() const
    {
        glm::mat4 model(1.0f);  // Matriz identidad
        model = glm::translate(model, position);  // Aplicar traslación
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));  // Rotar en X
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));  // Rotar en Y
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));  // Rotar en Z
        model = glm::scale(model, scale);  // Aplicar escala
        return model;
    }
}
