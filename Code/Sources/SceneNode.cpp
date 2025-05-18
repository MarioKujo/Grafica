// SceneNode.cpp
#include "../Headers/SceneNode.hpp"

namespace udit {
    SceneNode::SceneNode(Object* obj)
        : object(obj), position(0.f), rotation(0.f), scale(1.f) {}

    void SceneNode::addChild(std::shared_ptr<SceneNode> child) {
        children.push_back(child);
    }

    void SceneNode::setTransform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) {
        position = pos;
        rotation = rot;
        scale = scl;
    }

    glm::mat4 SceneNode::computeLocalTransform() const {
        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        model = glm::scale(model, scale);
        return model;
    }

    void SceneNode::render(const glm::mat4& parentTransform, const glm::mat4& view, const glm::mat4& projection) {
        glm::mat4 local = computeLocalTransform();
        glm::mat4 global = parentTransform * local;

        if (object) {
            // Solo para el objeto actual, pasamos el model transform ya acumulado
            glm::mat4 modelView = view * global;
            object->getShader()->use();
            object->getShader()->setMat4("model_view_matrix", modelView);
            object->getShader()->setMat4("projection_matrix", projection);
            object->getMesh()->render();
        }

        for (auto& child : children) {
            child->render(global, view, projection);
        }
    }
}
