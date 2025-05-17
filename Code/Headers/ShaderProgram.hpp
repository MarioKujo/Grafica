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

        /**
         * @brief Muestra los errores de compilación de un shader.
         *
         * Si ocurre un error durante la compilación de un shader, este método muestra
         * los detalles del error para facilitar la depuración.
         *
         * @param shader_id El identificador del shader cuya compilación falló.
         */
        void show_compilation_error(GLuint shader_id);

        /**
         * @brief Muestra los errores de enlace de un programa de shaders.
         *
         * Si ocurre un error al enlazar los shaders (vértices y fragmentos)
         * en un solo programa, este método muestra los detalles del error.
         *
         * @param program_id El identificador del programa de shaders cuyo enlace falló.
         */
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