#include "../Headers/SceneNode.hpp"

namespace udit
{

    // Constructor: Inicializa el nodo con un objeto y valores por defecto para posición, rotación y escala
    SceneNode::SceneNode(Object* obj)
        : object(obj), position(0.0f), rotation(0.0f), scale(1.0f) {}

    // Añade un nodo hijo a la lista de hijos
    void SceneNode::addChild(std::shared_ptr<SceneNode> child)
    {
        children.push_back(std::move(child));
    }

    // Establece la transformación local (posición, rotación, escala)
    void SceneNode::setTransform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl)
    {
        position = pos;
        rotation = rot;
        scale = scl;
    }

    // Calcula la matriz de transformación local combinando traslación, rotación y escala
    glm::mat4 SceneNode::computeLocalTransform() const
    {
        glm::mat4 transform = glm::mat4(1.0f);            // Matriz identidad
        transform = glm::translate(transform, position);  // Traslación

        // Aplicar rotaciones en X, Y y Z en ese orden
        transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));

        transform = glm::scale(transform, scale);         // Escala
        return transform;
    }

    // Renderiza este nodo y todos sus hijos recursivamente
    void SceneNode::render(const glm::mat4& parentTransform, const glm::mat4& view, const glm::mat4& projection)
    {
        glm::mat4 localTransform = computeLocalTransform();
        glm::mat4 globalTransform = parentTransform * localTransform;

        if (object)
        {
            // Renderizar el objeto con la matriz model-view acumulada
            glm::mat4 modelView = view * globalTransform;
            object->render(modelView, projection);
        }

        // Renderizar todos los nodos hijos con la transformación global actualizada
        for (const auto& child : children)
        {
            child->render(globalTransform, view, projection);
        }
    }

}