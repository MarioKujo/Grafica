#pragma once
#include "Mesh.hpp"

namespace udit
{
    /**
     * @brief Representa un objeto 3D con una malla, shader y transformaciones.
     *
     * Permite configurar posición, rotación, escala, texturas y renderizar el objeto.
     */
    class Object {
    public:
        /**
         * @brief Constructor que inicializa el objeto con una malla y un shader.
         * @param mesh Puntero a la malla a renderizar.
         * @param shader Puntero al programa de shader para renderizar el objeto.
         */
        Object(Mesh* mesh, ShaderProgram* shader);

        /**
         * @brief Renderiza el objeto usando las matrices de vista y proyección dadas.
         *
         * Configura texturas, activa blending si es necesario, y envía las matrices al shader.
         * @param view Matriz de vista (camera).
         * @param projection Matriz de proyección.
         */
        void render(const glm::mat4& view, const glm::mat4& projection);

        /**
         * @brief Configura las texturas y el blending para la renderización,
         *        seleccionando el método adecuado según la presencia de
         *        heightmap o transparencia.
         */
        void setupTexturesAndBlending();

        /**
         * @brief Vincula la textura del heightmap y la textura principal,
         *        configurando los uniformes y activando las unidades de textura necesarias.
         */
        void bindHeightmapAndTexture();

        /**
         * @brief Vincula la textura principal y habilita el blending para
         *        soportar transparencia, ajustando el estado de profundidad y mezcla.
         */
        void bindTextureWithTransparency();

        /**
         * @brief Vincula solo la textura principal sin blending ni heightmap.
         */
        void bindTextureOnly();

        /**
         * @brief Limpia el estado de blending y máscara de profundidad después
         *        de renderizar objetos con transparencia.
         */
        void cleanupBlending();


        /**
         * @brief Establece la posición del objeto en el espacio 3D.
         * @param pos Vector 3D con la nueva posición.
         */
        void setPosition(const glm::vec3& pos);

        /**
         * @brief Establece la rotación del objeto en grados sobre cada eje.
         * @param rot Vector 3D con la rotación en grados (x, y, z).
         */
        void setRotation(const glm::vec3& rot);

        /**
         * @brief Establece la escala del objeto en cada eje.
         * @param scl Vector 3D con los factores de escala.
         */
        void setScale(const glm::vec3& scl);

        /**
         * @brief Asigna la textura principal (color) del objeto.
         * @param texture ID de la textura OpenGL.
         */
        void setTextureID(GLuint texture);

        /**
         * @brief Asigna la textura del mapa de altura para efectos de desplazamiento.
         * @param texture ID de la textura OpenGL para el mapa de altura.
         */
        void setHeightmapTextureID(GLuint texture);

        /**
         * @brief Obtiene el shader asociado al objeto.
         * @return Puntero al ShaderProgram.
         */
        ShaderProgram* getShader() const;

        /**
         * @brief Obtiene la malla asociada al objeto.
         * @return Puntero a la malla.
         */
        Mesh* getMesh() const;

    private:
        Mesh* mesh;                  ///< Puntero a la malla del objeto
        ShaderProgram* shader;       ///< Puntero al programa de shader utilizado

        GLuint textureID;            ///< ID de la textura principal
        GLuint heightmapID;          ///< ID de la textura del mapa de altura

        GLint heightmapLoc;          ///< Localización del uniform "heightmap" en el shader
        GLint transparencyLoc;       ///< Localización del uniform "transparency" en el shader

        glm::vec3 position;          ///< Posición del objeto en el espacio 3D
        glm::vec3 rotation;          ///< Rotación del objeto (en grados) sobre cada eje
        glm::vec3 scale;             ///< Escala del objeto en cada eje

        /**
         * @brief Calcula la matriz modelo combinando traslación, rotación y escala.
         * @return Matriz modelo 4x4.
         */
        glm::mat4 computeModelMatrix() const;
    };
}
