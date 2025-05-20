#pragma once
#include "Object.hpp"
#include <vector>
#include <memory>
#include <gtc/matrix_transform.hpp>

namespace udit
{

    /**
     * @class SceneNode
     * @brief Nodo de escena para jerarquía de objetos 3D.
     *
     * Representa un nodo en un grafo de escena, que puede contener un objeto
     * y varios nodos hijos, con transformaciones locales acumulables.
     */
    class SceneNode
    {
    public:
        /**
         * @brief Constructor.
         * @param object Puntero a un objeto que este nodo representa. Puede ser nullptr.
         */
        SceneNode(Object* object = nullptr);

        /**
         * @brief Añade un nodo hijo a este nodo.
         * @param child Nodo hijo a añadir.
         */
        void addChild(std::shared_ptr<SceneNode> child);

        /**
         * @brief Define la transformación local del nodo.
         * @param position Vector de posición local.
         * @param rotation Vector de rotación local en grados (euler angles).
         * @param scale Vector de escala local.
         */
        void setTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

        /**
         * @brief Renderiza el nodo y sus hijos.
         * @param parentTransform Matriz de transformación acumulada del nodo padre.
         * @param view Matriz de vista de la cámara.
         * @param projection Matriz de proyección.
         *
         * Se calcula la transformación global y se pasa a los objetos para renderizar.
         */
        void render(const glm::mat4& parentTransform, const glm::mat4& view, const glm::mat4& projection);

    private:
        Object* object; ///< Objeto representado en este nodo (puede ser nullptr).
        glm::vec3 position; ///< Posición local.
        glm::vec3 rotation; ///< Rotación local (grados).
        glm::vec3 scale;    ///< Escala local.

        std::vector<std::shared_ptr<SceneNode>> children; ///< Nodos hijos.

        /**
         * @brief Calcula la matriz de transformación local (modelo).
         * @return Matriz 4x4 que representa la transformación local.
         */
        glm::mat4 computeLocalTransform() const;
    };

}
