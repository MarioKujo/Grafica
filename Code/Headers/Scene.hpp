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
   
    class Scene
    {
    private:
        // Programa de shaders
        ShaderProgram defaultProgram;
        ShaderProgram unlitProgram;
        ShaderProgram skyboxProgram;
        ShaderProgram heightmapProgram;

        static const float UFO_HEIGHT;
        static const float COW_HEIGHT;
        static const float CONE_HEIGHT;
        static const float SCALE_SMALL;
        static const float SCALE_BIG;

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
        std::shared_ptr<SceneNode> ufoCowConeNode;
        std::shared_ptr<SceneNode> ufoNode;
        std::shared_ptr<SceneNode> cowNode;
        std::shared_ptr<SceneNode> coneNode;
        std::shared_ptr<SceneNode> heightmapNode;

    public:

        Scene(unsigned width, unsigned height);

        void setGraph();

        void loadTextures();

        void setTextures();

        void update();

        
        void render();

        void lightSetup(glm::mat4& view_matrix);

        void resize(unsigned width, unsigned height);

        void set_camera(Camera new_camera);
    };
}
