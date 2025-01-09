/**
 * @file Scene.hpp
 * @brief Declaración de la clase Scene para gestionar una escena en OpenGL.
 *
 * Esta clase incluye funcionalidades para manejar la cámara, renderizar objetos 3D,
 * procesar eventos y actualizar el estado de la escena.
 *
 * @author Angel Rodriguez
 * @date Public domain
 */

#pragma once

#include "Cylinder.hpp"
#include "Cone.hpp"
#include "Plane.hpp"
#include "Cube.hpp"
#include "Camera.hpp"
#include "TextureLoader.hpp"
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
     * @brief Clase para gestionar una escena con OpenGL.
     *
     * Contiene métodos para actualizar, renderizar y manejar eventos en una escena.
     */
    class Scene
    {
    private:

        static const std::string vertex_shader_code;   ///< Código fuente del shader de vértices.
        static const std::string fragment_shader_code; ///< Código fuente del shader de fragmentos.

        GLint  model_view_matrix_id; ///< ID del uniforme para la matriz de vista-modelo.
        GLint  projection_matrix_id; ///< ID del uniforme para la matriz de proyección.
        GLuint program_id;
        Cube   cube; ///< Objeto Cube para renderizar.
        Cone cone; /**< Cono 3D que forma parte de la escena. */
        Cylinder cylinder; /**< Cilindro 3D que forma parte de la escena. */
        Plane plane; /**< Plano 3D que forma parte de la escena. */
        float  angle; ///< Ángulo de rotación del cubo.

        Camera camera; ///< Cámara para gestionar la vista y la proyección.

        TextureLoader textureLoader; ///< Instancia de TextureLoader para cargar texturas
        GLuint cubeTextureID; ///< ID de la textura del cubo
        GLuint planeTextureID; ///< ID de la textura del plano
        GLuint cylinderTextureID; ///< ID de la textura del cilindro
        GLuint coneTextureID; ///< ID de la textura del cono
    public:

        /**
         * @brief Constructor de Scene.
         *
         * Inicializa la escena con el ancho y alto especificados.
         *
         * @param width Ancho del viewport.
         * @param height Alto del viewport.
         */
        Scene(unsigned width, unsigned height);

        /**
         * @brief Actualiza el estado de la escena.
         *
         * Permite animaciones y otros cambios en función del tiempo transcurrido.
         *
         * @param delta_time Tiempo transcurrido desde la última actualización (en segundos).
         */
        void update(float delta_time);

        /**
         * @brief Renderiza la escena.
         *
         * Dibuja los objetos en el contexto de OpenGL.
         */
        void render();

        /**
         * @brief Ajusta el tamaño del viewport.
         *
         * Permite actualizar las proyecciones al cambiar las dimensiones de la ventana.
         *
         * @param width Nuevo ancho del viewport.
         * @param height Nuevo alto del viewport.
         */
        void resize(unsigned width, unsigned height);

        /**
         * @brief Maneja los movimientos del ratón.
         *
         * Actualiza la orientación de la cámara en respuesta al movimiento relativo del ratón.
         *
         * @param xrel Movimiento relativo en el eje X.
         * @param yrel Movimiento relativo en el eje Y.
         */
        void handle_mouse_motion(int xrel, int yrel);

    private:

        /**
         * @brief Compila los shaders necesarios para la escena.
         *
         * @return ID del programa de shaders compilado.
         */
        GLuint compile_shaders();

        /**
         * @brief Muestra errores de compilación de shaders.
         *
         * @param shader_id ID del shader con errores.
         */
        void show_compilation_error(GLuint shader_id);

        /**
         * @brief Muestra errores de enlace del programa de shaders.
         *
         * @param program_id ID del programa con errores.
         */
        void show_linkage_error(GLuint program_id);

    };

}
