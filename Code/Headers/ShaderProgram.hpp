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

		/**
         * @brief Compila los shaders para la escena.
         *
         * Compila los shaders de vértices y fragmentos para la escena 3D.
         * Los shaders definen cómo se procesan los vértices y los fragmentos (pixeles)
         * de los objetos 3D renderizados en la escena.
         *
         * @return El identificador del programa de shaders compilado.
         */
        GLuint compile_shaders(string vertex_shader, string fragment_shader);
	};
}