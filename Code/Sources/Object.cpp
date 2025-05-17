#include "../Headers/Object.hpp"

namespace udit
{
    Object::Object(Mesh* m, ShaderProgram* s)
        : mesh(m), shader(s), position(0), rotation(0), scale(1)
    {}

    void Object::render(const glm::mat4& view, const glm::mat4& projection)
    {
        shader->use();
        glm::mat4 model = computeModelMatrix();
        shader->setMat4("model_view_matrix", (view * model));
        shader->setMat4("projection_matrix", projection);
        mesh->render();
    }

    void Object::setPosition(const glm::vec3& pos) { position = pos; }
    void Object::setRotation(const glm::vec3& rot) { rotation = rot; }
    void Object::setScale(const glm::vec3& scl) { scale = scl; }

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
