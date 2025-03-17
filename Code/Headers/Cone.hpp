/*@file Cone.hpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Definición de la clase Cone que representa un cono 3D.
 *
 * La clase Cone se utiliza para generar y renderizar un cono 3D en OpenGL. Permite
 * especificar el número de divisiones, el radio de la base y la altura del cono.
 * También gestiona los VBOs y VAOs necesarios para la representación del cono.
 */

#pragma once
#include "Mesh.hpp"

namespace udit
{
    /**
     * @class Cone
     * @brief Representa un cono 3D con un número de divisiones, radio y altura.
     *
     * La clase permite la creación de un cono 3D, la gestión de sus geometrías
     * (coordenadas, índices y coordenadas de textura) y el renderizado del mismo
     * utilizando OpenGL. El cono se genera a partir de las especificaciones de
     * radio, altura y divisiones.
     */
    class Cone : public Mesh
    {
    public:
        /**
         * @brief Constructor de la clase Cone.
         *
         * Inicializa el cono con el número de divisiones, el radio y la altura
         * especificados. Este constructor también prepara las estructuras necesarias
         * para el renderizado.
         *
         * @param d Número de divisiones del cono.
         * @param r Radio de la base del cono.
         * @param h Altura del cono.
         */
        Cone(int d, GLfloat r, GLfloat h);

        /**
         * @brief Destructor de la clase Cone.
         *
         * Libera los recursos utilizados por los buffers de OpenGL (VAO y VBOs).
         */
        ~Cone();

    private:
        int divisions; /**< Número de divisiones que se utilizarán para crear el cono. */
        GLfloat radius; /**< Radio de la base del cono. */
        GLfloat height; /**< Altura del cono. */

        void generateGeometry();
    };
}
