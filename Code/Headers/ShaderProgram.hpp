#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <string>
using namespace std;
namespace udit
{
	class ShaderProgram
	{
	private:
        GLuint program_id;

        void show_compilation_error(GLuint shader_id);

        void show_linkage_error(GLuint program_id);

    public:
        ShaderProgram(const std::string& vertex_source, const std::string& fragment_source);
        ~ShaderProgram();

        void use() const;
        GLuint id() const { return program_id; }

        // Métodos para subir uniforms
        void setMat4(const std::string& name, const glm::mat4& mat) const;
        void setVec3(const std::string& name, const glm::vec3& vec) const;
        void setFloat(const std::string& name, float value) const;
        void setInt(const std::string& name, int value) const;
	};
}