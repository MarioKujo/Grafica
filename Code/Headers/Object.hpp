#include "Mesh.hpp"
#include "ShaderProgram.hpp"
namespace udit
{
    class Object {
    public:
        Object(Mesh* mesh, ShaderProgram* shader);

        void render(const glm::mat4& view, const glm::mat4& projection);
        void setPosition(const glm::vec3& pos);
        void setRotation(const glm::vec3& rot);
        void setScale(const glm::vec3& scl);
        ShaderProgram* getShader() const;
        Mesh* getMesh() const;
    private:
        Mesh* mesh;
        ShaderProgram* shader;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        glm::mat4 computeModelMatrix() const;
    };
}
