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

#include "AssimpMesh.hpp"
#include "Camera.hpp"
#include "TextureLoader.hpp"
#include "Skybox.hpp"
#include "SceneNode.hpp"
#include <cassert>
#include <SDL.h>

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
        // Programa de shaders
        ShaderProgram defaultProgram;
        ShaderProgram unlitProgram;
        ShaderProgram skyboxProgram;
        ShaderProgram heightmapProgram;


        // Propiedades de la luz
        glm::vec3 lightPos;  // Posición de la luz
        glm::vec3 lightColor;  // Color blanco para la luz
        glm::vec3 viewPos;  // Posición de la cámara


        // Códigos de los shaders
        static const string vertex_shader_code; ///< Código fuente del shader de vértices.
        static const string fragment_shader_code; ///< Código fuente del shader de fragmentos.

        static const string vertex_shader_unlit_code; ///< Código fuente del shader de vértices.
        static const string fragment_shader_unlit_code; ///< Código fuente del shader de fragmentos.

        static const string skybox_vertex_shader; ///< Código fuente del shader de vértices para el skybox.
        static const string skybox_fragment_shader; ///< Código fuente del shader de fragmentos para el skybox.

        static const string heightmap_vertex_shader; ///< Código fuente del shader de vértices para el skybox.
        static const string heightmap_fragment_shader; ///< Código fuente del shader de fragmentos para el skybox.

        glm::mat4 projection_matrix;

        // Objetos 3D de la escena
        Skybox skybox; ///< Objeto para representar el skybox.

        AssimpMesh ufo; ///< Objeto UFO.
        AssimpMesh cow; ///< Objeto UFO.

        Mesh plane;
        Mesh cone;

        GeometryGenerator generator;

        Object heightmapObj;
        Object ufoObj;
        Object cowObj;
        Object coneObj;

        // Ángulo de rotación de la escena.
        float angle; ///< Ángulo de rotación para objetos en la escena.

        // Cámara que gestiona la vista.
        Camera camera; ///< Cámara que define la perspectiva de la escena.

        // Gestor de texturas
        TextureLoader textureLoader; ///< Cargador de texturas para los objetos 3D.

        // Identificadores de las texturas para cada objeto 3D
        GLuint coneTextureID; ///< ID de la textura para el cono.
        GLuint skyboxTextureID; ///< ID de la textura para el skybox.
        GLuint heightmapID; ///< ID de la textura para el heightmap.
        GLuint heightmapTextureID; ///< ID de la textura decorativa para el heightmap.

        GLuint cowTextureID;
        GLuint ufoTextureID; ///< ID de la textura decorativa para el UFO.

        std::shared_ptr<SceneNode> rootNode;
        std::shared_ptr<SceneNode> ufoNode;
        std::shared_ptr<SceneNode> cowNode;
        std::shared_ptr<SceneNode> coneNode;
        std::shared_ptr<SceneNode> heightmapNode;

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

        void setGraph();

        void loadTextures();

        /**
         * @brief Actualiza la escena.
         *
         * Este método actualiza los elementos de la escena en función del tiempo transcurrido,
         * incluyendo las animaciones de los objetos y los movimientos de la cámara.
         *
         * @param delta_time Tiempo transcurrido desde la última actualización.
         */
        void update();

        /**
         * @brief Renderiza la escena.
         *
         * Este método dibuja todos los objetos 3D de la escena utilizando OpenGL,
         * aplicando las transformaciones y shaders correspondientes.
         */
        void render();

        void renderUFO(glm::mat4& view_matrix, float y_offset);

        void renderCow(glm::mat4& view_matrix, float y_offset);

        void renderHeightmap(glm::mat4& view_matrix);

        void renderCone(glm::mat4& view_matrix, float y_offset);

        void lightSetup(glm::mat4& view_matrix);

        void renderSkybox(glm::mat4& view_matrix);

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
        * @brief Recibe una Cámara y cambia los valores de la cámara de la escena
        */
        void set_camera(Camera new_camera);
    };
}
