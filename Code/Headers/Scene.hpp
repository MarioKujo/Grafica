/**
 * @file Scene.hpp
 * @author Original Author <angel.rodriguez@udit.es>
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Definición de la clase Scene, encargada de manejar y renderizar la escena 3D.
 *
 * Esta clase es responsable de gestionar los objetos 3D en la escena, las texturas, las cámaras, y los shaders.
 * También se encarga de las operaciones de actualización y renderizado de la escena utilizando OpenGL.
 *
 * @note Modificado por andrmatgonros@gmail.com para extender la funcionalidad original de la clase Scene.
 *       La versión original sólo renderizaba un cubo girando. Se agregaron los siguientes elementos:
 *       - Skybox: Representación de un entorno 3D que rodea la escena.
 *       - Cámara: Implementación de una cámara interactiva para moverse por la escena.
 *       - Nuevos objetos 3D: Se añadieron un plano, un cilindro, un cono, una esfera y un heightmap.
 *       - TextureLoader: Creación de un cargador de texturas personalizado para mejorar la carga de texturas en la escena.
 */

#pragma once

#include "Cylinder.hpp"
#include "Heightmap.hpp"
#include "Cone.hpp"
#include "Plane.hpp"
#include "Camera.hpp"
#include "TextureLoader.hpp"
#include "Skybox.hpp"
#include "Sphere.hpp"
#include <string>
#include <iostream>
#include <cassert>
#include <SDL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace udit
{
    /**
     * @class Scene
     * @brief Clase para representar y gestionar una escena 3D.
     *
     * Esta clase encapsula la creación de objetos 3D (como esferas, conos, cilindros, planos, etc.), la configuración
     * de la cámara, la carga de texturas, y la administración de los shaders para la representación visual de la escena.
     * Permite renderizar la escena y actualizarla durante la ejecución del programa.
     *
     * @note Esta clase ha sido modificada respecto a su versión original para incluir:
     *       - Un entorno 3D con un skybox.
     *       - La implementación de una cámara interactiva.
     *       - Nuevos objetos como un cilindro, un cono, una esfera, un plano y un heightmap.
     *       - Un cargador de texturas personalizado para los objetos 3D.
     */
    class Scene
    {
    private:
        // Códigos de los shaders
        static const std::string vertex_shader_code; ///< Código fuente del shader de vértices.
        static const std::string fragment_shader_code; ///< Código fuente del shader de fragmentos.
        static const std::string skybox_vertex_shader; ///< Código fuente del shader de vértices para el skybox.
        static const std::string skybox_fragment_shader; ///< Código fuente del shader de fragmentos para el skybox.
        static const std::string heightmap_vertex_shader; ///< Código fuente del shader de vértices para el heightmap.
        static const std::string heightmap_fragment_shader; ///< Código fuente del shader de fragmentos para el heightmap.

        // Identificadores de las matrices y programas de los shaders
        GLint model_view_matrix_id; ///< ID de la matriz de vista y modelo para los shaders.
        GLint projection_matrix_id; ///< ID de la matriz de proyección para los shaders.
        GLuint program_id; ///< ID del programa principal de shaders.
        GLuint skybox_program_id; ///< ID del programa de shaders para el skybox.

        // Objetos 3D de la escena
        Skybox skybox; ///< Objeto para representar el skybox.
        Sphere sphere1; ///< Primer objeto esfera.
        Sphere sphere2; ///< Segundo objeto esfera.
        Cone cone; ///< Objeto cono.
        Cylinder cylinder; ///< Objeto cilindro.
        Plane plane; ///< Objeto plano.
        Heightmap heightmap; ///< Objeto heightmap para representar terrenos.

        // Ángulo de rotación de la escena.
        float angle; ///< Ángulo de rotación para objetos en la escena.

        // Cámara que gestiona la vista.
        Camera camera; ///< Cámara que define la perspectiva de la escena.

        // Gestor de texturas
        TextureLoader textureLoader; ///< Cargador de texturas para los objetos 3D.

        // Identificadores de las texturas para cada objeto 3D
        GLuint cubeTextureID; ///< ID de la textura para el cubo.
        GLuint planeTextureID; ///< ID de la textura para el plano.
        GLuint cylinderTextureID; ///< ID de la textura para el cilindro.
        GLuint coneTextureID; ///< ID de la textura para el cono.
        GLuint skyboxTextureID; ///< ID de la textura para el skybox.
        GLuint sphereTextureID; ///< ID de la textura para la esfera.
        GLuint heightmapTextureID; ///< ID de la textura para el heightmap.

        /**
         * @brief Compila los shaders para la escena.
         *
         * Compila los shaders de vértices y fragmentos para la escena 3D.
         * Los shaders definen cómo se procesan los vértices y los fragmentos (pixeles)
         * de los objetos 3D renderizados en la escena.
         *
         * @return El identificador del programa de shaders compilado.
         */
        GLuint compile_shaders();

        /**
         * @brief Compila los shaders para el skybox.
         *
         * Compila los shaders de vértices y fragmentos específicos para el skybox,
         * que se usa para simular un entorno 3D alrededor de la escena.
         *
         * @return El identificador del programa de shaders para el skybox compilado.
         */
        GLuint compile_skybox_shaders();

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
         * @brief Constructor de la clase Scene.
         *
         * Inicializa la escena con los objetos 3D, la cámara, las texturas y los shaders
         * necesarios para renderizar la escena correctamente.
         *
         * @param width Ancho de la ventana de la escena.
         * @param height Alto de la ventana de la escena.
         */
        Scene(unsigned width, unsigned height);

        /**
         * @brief Actualiza la escena.
         *
         * Este método actualiza los elementos de la escena en función del tiempo transcurrido,
         * incluyendo las animaciones de los objetos y los movimientos de la cámara.
         *
         * @param delta_time Tiempo transcurrido desde la última actualización.
         */
        void update(float delta_time);

        /**
         * @brief Renderiza la escena.
         *
         * Este método dibuja todos los objetos 3D de la escena utilizando OpenGL,
         * aplicando las transformaciones y shaders correspondientes.
         */
        void render();

        /**
         * @brief Cambia el tamaño de la ventana de la escena.
         *
         * Ajusta la relación de aspecto y las matrices de proyección en función del nuevo tamaño
         * de la ventana para asegurar una correcta visualización de la escena.
         *
         * @param width Nuevo ancho de la ventana.
         * @param height Nuevo alto de la ventana.
         */
        void resize(unsigned width, unsigned height);

        /**
         * @brief Maneja el movimiento del ratón en la ventana.
         *
         * Permite controlar la cámara con el ratón, ajustando su orientación.
         *
         * @param xrel Movimiento del ratón en el eje X.
         * @param yrel Movimiento del ratón en el eje Y.
         */
        void handle_mouse_motion(float xrel, float yrel);
    };
}
