// Este código es de dominio público
// angel.rodriguez@udit.es

#pragma once
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "Plane.hpp"
#include <string>

namespace udit
{

    /**
     * @brief Clase que representa una escena 3D con un cilindro y un plano.
     *
     * La clase encapsula la lógica de renderizado y manejo de shaders para mostrar
     * la escena con los objetos incluidos.
     */
    class Scene
    {
    private:

        /**
         * @brief Código fuente del vertex shader.
         */
        static const std::string vertex_shader_code;

        /**
         * @brief Código fuente del fragment shader.
         */
        static const std::string fragment_shader_code;

        GLint model_view_matrix_id; /**< ID del uniform de la matriz de vista/modelo. */
        GLint projection_matrix_id; /**< ID del uniform de la matriz de proyección. */

        Cone cone; /**< Cono 3D que forma parte de la escena. */
        Cylinder cylinder; /**< Cilindro 3D que forma parte de la escena. */
        Plane plane; /**< Plano 3D que forma parte de la escena. */
        float angle; /**< Ángulo de rotación de la escena. */

    public:

        /**
         * @brief Constructor de la clase Scene.
         *
         * Inicializa los recursos y configura la escena.
         *
         * @param width Ancho inicial de la ventana.
         * @param height Alto inicial de la ventana.
         */
        Scene(unsigned width, unsigned height);

        /**
         * @brief Actualiza los parámetros de la escena, como animaciones o transformaciones.
         */
        void update();

        /**
         * @brief Renderiza la escena en pantalla.
         */
        void render();

        /**
         * @brief Ajusta el tamaño de la proyección para adaptarse a una ventana redimensionada.
         *
         * @param width Nuevo ancho de la ventana.
         * @param height Nuevo alto de la ventana.
         */
        void resize(unsigned width, unsigned height);

    private:

        /**
         * @brief Compila los shaders utilizados por la escena.
         *
         * Crea y enlaza un programa de shaders utilizando los códigos fuente de
         * los shaders especificados.
         *
         * @return GLuint ID del programa de shaders compilado.
         */
        GLuint compile_shaders();

        /**
         * @brief Muestra un mensaje de error en caso de que falle la compilación de un shader.
         *
         * @param shader_id ID del shader que falló durante la compilación.
         */
        void show_compilation_error(GLuint shader_id);

        /**
         * @brief Muestra un mensaje de error en caso de que falle el enlace de un programa de shaders.
         *
         * @param program_id ID del programa que falló durante el enlace.
         */
        void show_linkage_error(GLuint program_id);

    };

}
