/*@file Cylinder.hpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Definición de la clase Cylinder que representa un cilindro en un espacio 3D.
 *
 * Este archivo contiene la declaración de la clase Cylinder, que se encarga de la generación
 * de la geometría de un cilindro, la configuración de los buffers necesarios para su renderizado
 * utilizando OpenGL y su renderización en pantalla.
 */

#pragma once
#include "Mesh.hpp"

namespace udit
{
    /**
     * @class Cylinder
     * @brief Clase que representa un cilindro en un espacio 3D.
     *
     * La clase se encarga de generar la geometría de un cilindro, con sus vértices,
     * coordenadas de textura e índices, y de renderizarlo utilizando OpenGL.
     */
    class Cylinder: public Mesh
    {
    public:
        /**
         * @brief Constructor de la clase Cylinder.
         *
         * Este constructor inicializa los parámetros del cilindro, como la cantidad
         * de pilas (stacks), la cantidad de segmentos (slices), el radio y la altura,
         * y luego genera la geometría asociada a esos parámetros.
         *
         * @param stack Número de divisiones verticales del cilindro (pilas).
         * @param slice Número de divisiones alrededor del eje del cilindro (segmentos).
         * @param r Radio de la base del cilindro.
         * @param h Altura del cilindro.
         */
        Cylinder(int stack, int slice, GLfloat r, GLfloat h);

        /**
         * @brief Destructor de la clase Cylinder.
         *
         * Este destructor se encarga de liberar los recursos utilizados por el cilindro,
         * como los buffers de OpenGL.
         */
        ~Cylinder();

    private:
        int stack_count; /**< Número de divisiones verticales del cilindro. */
        int slice_count; /**< Número de divisiones alrededor del eje del cilindro. */
        GLfloat radius; /**< Radio de la base del cilindro. */
        GLfloat height; /**< Altura del cilindro. */

        /**
         * @brief Genera la geometría del cilindro.
         *
         * Este método calcula los vértices, las coordenadas de textura y los índices
         * necesarios para representar el cilindro con la cantidad de pilas y segmentos
         * especificados.
         */
        void generateGeometry();
    };
}
