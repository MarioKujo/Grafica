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
     * @brief Clase que representa una escena 3D completa con objetos, cámara, luz y shaders.
     *
     * Gestiona la carga, configuración, actualización y renderizado de todos los elementos 3D,
     * así como la configuración de iluminación y cámara.
     */
    class Scene
    {
    private:
        ShaderProgram defaultProgram; ///< Programa de shaders con iluminación.
        ShaderProgram unlitProgram;   ///< Programa de shaders sin iluminación.
        ShaderProgram skyboxProgram;  ///< Programa de shaders para skybox.
        ShaderProgram heightmapProgram; ///< Programa de shaders para heightmap.

        static const float UFO_HEIGHT;    ///< Altura del UFO.
        static const float COW_HEIGHT;    ///< Altura de la vaca.
        static const float CONE_HEIGHT;   ///< Altura del cono.
        static const float SCALE_SMALL;   ///< Escala pequeña para ciertos objetos.
        static const float SCALE_BIG;     ///< Escala grande para ciertos objetos.

        glm::vec3 lightPos;   ///< Posición de la luz principal.
        glm::vec3 lightColor; ///< Color de la luz (blanco).
        glm::vec3 viewPos;    ///< Posición de la cámara.

        glm::mat4 projection_matrix; ///< Matriz de proyección de la cámara.

        Skybox skybox;           ///< Objeto skybox para fondo de la escena.

        AssimpMesh ufo;          ///< Malla 3D del OVNI.
        AssimpMesh cow;          ///< Malla 3D de la vaca.

        Mesh plane;              ///< Malla plana para el heightmap.
        Mesh cone;               ///< Malla del cono.

        GeometryGenerator generator; ///< Generador de geometría para primitivas.

        Object heightmapObj; ///< Objeto heightmap.
        Object ufoObj;       ///< Objeto OVNI.
        Object cowObj;       ///< Objeto vaca.
        Object coneObj;      ///< Objeto cono.

        float angle; ///< Ángulo de rotación para animar la escena.

        Camera camera; ///< Cámara que define la vista y perspectiva.

        TextureLoader textureLoader; ///< Gestor para cargar texturas.

        GLuint coneTextureID;        ///< ID de la textura para el cono.
        GLuint skyboxTextureID;      ///< ID de la textura para el skybox.
        GLuint heightmapID;          ///< ID de la textura heightmap (altura).
        GLuint heightmapTextureID;   ///< ID de la textura decorativa para heightmap.
        GLuint cowTextureID;         ///< ID de la textura para la vaca.
        GLuint ufoTextureID;         ///< ID de la textura para el OVNI.

        std::shared_ptr<SceneNode> rootNode;         ///< Nodo raíz de la escena.
        std::shared_ptr<SceneNode> ufoCowConeNode;   ///< Nodo contenedor para UFO, vaca y cono.
        std::shared_ptr<SceneNode> ufoNode;          ///< Nodo para el UFO.
        std::shared_ptr<SceneNode> cowNode;          ///< Nodo para la vaca.
        std::shared_ptr<SceneNode> coneNode;         ///< Nodo para el cono.
        std::shared_ptr<SceneNode> heightmapNode;    ///< Nodo para el heightmap.

    public:

        /**
         * @brief Constructor de la escena.
         *
         * Inicializa los objetos 3D, cámaras, shaders y texturas.
         *
         * @param width Ancho de la ventana.
         * @param height Alto de la ventana.
         */
        Scene(unsigned width, unsigned height);

        /**
         * @brief Construye el grafo de la escena configurando los nodos y sus transformaciones.
         */
        void setGraph();

        /**
         * @brief Carga todas las texturas necesarias para la escena.
         */
        void loadTextures();

        /**
         * @brief Asigna las texturas cargadas a los objetos correspondientes.
         */
        void setTextures();

        /**
         * @brief Actualiza la escena, animando el ángulo de rotación u otros parámetros.
         */
        void update();

        /**
         * @brief Renderiza la escena completa, incluyendo skybox y todos los nodos.
         */
        void render();

        /**
         * @brief Configura la iluminación en los shaders según la posición y color de las luces.
         *
         * @param view_matrix Matriz de vista actual para transformar las luces a espacio de cámara.
         */
        void lightSetup(glm::mat4& view_matrix);

        /**
         * @brief Ajusta el tamaño de la ventana y actualiza la matriz de proyección.
         *
         * @param width Nuevo ancho de la ventana.
         * @param height Nuevo alto de la ventana.
         */
        void resize(unsigned width, unsigned height);

        /**
         * @brief Establece una nueva cámara para la escena.
         *
         * @param new_camera Nueva instancia de la cámara.
         */
        void set_camera(Camera new_camera);
    };
}
