#pragma once
#include <vector>
#include <glm.hpp>
#include <glad/glad.h>
#include <numbers>

namespace udit
{

    /**
     * @brief Estructura que contiene datos de una malla generada.
     *
     * Incluye coordenadas de vértices, coordenadas de textura, normales e índices.
     */
    struct MeshData
    {
        std::vector<GLfloat> coordinates; /**< Posiciones de vértices (x, y, z). */
        std::vector<GLfloat> texCoords;   /**< Coordenadas de textura (u, v). */
        std::vector<GLfloat> normals;     /**< Normales para iluminación (x, y, z). */
        std::vector<GLuint> indices;      /**< Índices para dibujar triángulos. */
    };

    /**
     * @brief Clase para generar primitivas geométricas básicas.
     *
     * Proporciona funciones estáticas para crear mallas de planos, conos y cubos.
     */
    class GeometryGenerator
    {
    public:
        /**
         * @brief Genera una malla de un plano subdividido.
         * @param hDivisions Número de subdivisiones horizontales.
         * @param vDivisions Número de subdivisiones verticales.
         * @param width Ancho total del plano.
         * @param height Alto total del plano.
         * @return MeshData con los datos de vértices, normales, UVs e índices.
         */
        static MeshData generatePlane(int hDivisions, int vDivisions, float width, float height);

        /**
         * @brief Genera una malla de un cono.
         * @param radius Radio de la base del cono.
         * @param height Altura del cono.
         * @param segments Número de segmentos para aproximar la base circular.
         * @return MeshData con vértices, normales, UVs e índices.
         */
        static MeshData generateCone(float radius, float height, int segments);

        /**
         * @brief Genera una malla de un cubo centrado en el origen.
         * @param size Tamaño del lado del cubo. Por defecto 1.0f.
         * @return MeshData con vértices, normales, UVs e índices.
         */
        static MeshData generateCube(float size = 1.0f);
    };

}
