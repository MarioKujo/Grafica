/**
 * @file Scene.hpp
 * @brief Declaración de la clase Scene que representa una escena 3D completa.
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
     * @brief Representa una escena 3D con cámara, objetos, skybox, texturas, iluminación y shaders.
     *
     * Esta clase se encarga de gestionar la creación, configuración, actualización y renderizado de una
     * escena 3D compuesta por distintos objetos jerárquicos. También configura el sistema de iluminación
     * y controla la cámara.
     */
    class Scene
    {
    private:

        GeometryGenerator generator; ///< Generador de primitivas geométricas.

        unique_ptr<ShaderProgram> defaultProgram;      ///< Shader con iluminación estándar.
        unique_ptr<ShaderProgram> unlitProgram;        ///< Shader sin iluminación (para objetos como el cono).
        unique_ptr<ShaderProgram> skyboxProgram;       ///< Shader para renderizar el skybox.
        unique_ptr<ShaderProgram> heightmapProgram;    ///< Shader especializado en renderizar heightmaps.

        static const float UFO_HEIGHT;     ///< Altura base del modelo OVNI.
        static const float COW_HEIGHT;     ///< Altura base del modelo vaca.
        static const float CONE_HEIGHT;    ///< Altura base del modelo cono.
        static const float SCALE_SMALL;    ///< Escala reducida para modelos pequeños.
        static const float SCALE_BIG;      ///< Escala ampliada para modelos grandes.

        glm::vec3 lightPos;     ///< Posición de la luz principal (no usada directamente si se usan direcciones).
        glm::vec3 lightColor;   ///< Color de la luz.
        glm::vec3 viewPos;      ///< Posición de la cámara en el espacio de mundo.

        glm::mat4 projection_matrix; ///< Matriz de proyección en perspectiva.


        unique_ptr<AssimpMesh> ufo;            ///< Modelo 3D del OVNI.
        unique_ptr<AssimpMesh> cow;            ///< Modelo 3D de la vaca.

        unique_ptr<Mesh> plane;                ///< Malla del plano (heightmap).
        unique_ptr<Mesh> cone;                 ///< Malla del cono.

        unique_ptr<Object> heightmapObj;       ///< Objeto gráfico del heightmap.
        unique_ptr<Object> ufoObj;             ///< Objeto gráfico del OVNI.
        unique_ptr<Object> cowObj;             ///< Objeto gráfico de la vaca.
        unique_ptr<Object> coneObj;            ///< Objeto gráfico del cono.
        unique_ptr<Skybox> skybox;             ///< Objeto que representa el skybox de fondo.

        float angle;               ///< Ángulo de rotación usado para animación.

        Camera camera;             ///< Cámara activa de la escena.

        TextureLoader textureLoader; ///< Gestor de carga de texturas.

        GLuint coneTextureID;        ///< ID de la textura del cono.
        GLuint skyboxTextureID;      ///< ID del cubemap del skybox.
        GLuint heightmapID;          ///< ID de la textura de altura.
        GLuint heightmapTextureID;   ///< ID de la textura difusa para el heightmap.
        GLuint cowTextureID;         ///< ID de la textura de la vaca.
        GLuint ufoTextureID;         ///< ID de la textura del OVNI.

        std::shared_ptr<SceneNode> rootNode;       ///< Nodo raíz de la jerarquía de escena.
        std::shared_ptr<SceneNode> ufoCowConeNode; ///< Nodo contenedor de OVNI, vaca y cono.
        std::shared_ptr<SceneNode> ufoNode;        ///< Nodo del OVNI.
        std::shared_ptr<SceneNode> cowNode;        ///< Nodo de la vaca.
        std::shared_ptr<SceneNode> coneNode;       ///< Nodo del cono.
        std::shared_ptr<SceneNode> heightmapNode;  ///< Nodo del terreno con heightmap.


        void initResources();
        void initObjects();
        void initSceneGraph();
    public:
        /**
         * @brief Constructor de Scene.
         * @param width Ancho de la ventana de visualización.
         * @param height Alto de la ventana de visualización.
         *
         * Inicializa la cámara, objetos, shaders, texturas y configura la jerarquía de escena.
         */
        Scene(unsigned width, unsigned height);

        ~Scene();

        /**
         * @brief Carga las texturas requeridas desde disco y las transfiere a la GPU.
         */
        void loadTextures();

        /**
         * @brief Asigna los identificadores de textura a los objetos 3D correspondientes.
         */
        void setTextures();

        /**
         * @brief Actualiza parámetros dinámicos de la escena (como rotaciones).
         */
        void update();

        /**
         * @brief Renderiza toda la escena, incluyendo skybox, terreno y objetos.
         */
        void render();

        void applyLightSettings(ShaderProgram& shader, const vector<glm::vec3>& dirsView, const vector<glm::vec3>& colors, const vector<float>& intensities);

        /**
         * @brief Configura las fuentes de luz y sus propiedades en los shaders activos.
         * @param view_matrix Matriz de vista desde la cámara.
         */
        void lightSetup(glm::mat4& view_matrix);

        /**
         * @brief Recalcula la matriz de proyección y ajusta el viewport tras un redimensionado.
         * @param width Nuevo ancho de la ventana.
         * @param height Nuevo alto de la ventana.
         */
        void resize(unsigned width, unsigned height);

        /**
         * @brief Establece una nueva instancia de cámara para la escena.
         * @param new_camera Objeto Camera que será usado para generar la vista.
         */
        void set_camera(Camera new_camera);
    };
}
