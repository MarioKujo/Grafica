// SceneNode.hpp
#pragma once
#include "Object.hpp"
#include <vector>
#include <memory>
#include <gtc/matrix_transform.hpp>

namespace udit {
    class SceneNode {
    public:
        SceneNode(Object* object = nullptr);

        void addChild(std::shared_ptr<SceneNode> child);
        void setTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
        void render(const glm::mat4& parentTransform, const glm::mat4& view, const glm::mat4& projection);

    private:
        Object* object;
        glm::vec3 position, rotation, scale;
        std::vector<std::shared_ptr<SceneNode>> children;

        glm::mat4 computeLocalTransform() const;
    };
}
