/**
 * @file Sphere.hpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este archivo define la clase `Sphere`, que se utiliza para generar y renderizar una esfera
 * 3D en OpenGL. La esfera se define a partir de divisiones latitudinales y longitudinales,
 * lo que permite controlar la resolución de la geometría de la esfera.
 */

#pragma once
#include "Mesh.hpp"

using namespace std;

namespace udit
{
    /**
     * @class Sphere
     * @brief Clase que representa una esfera 3D generada por subdivisiones esféricas.
     *
     * La esfera es una representación 3D que se genera mediante un proceso de subdivisión en
     * latitudes y longitudes, lo que da como resultado un conjunto de vértices, coordenadas
     * de textura y un índice para representar la geometría de la esfera. Esta clase permite
     * renderizar la esfera utilizando OpenGL.
     */
    class Sphere : public Mesh
    {
    public:
        /**
         * @brief Constructor de la clase Sphere.
         *
         * Este constructor recibe las divisiones latitudinales y longitudinales, así como el
         * radio de la esfera. Utiliza estos parámetros para generar la geometría de la esfera.
         *
         * @param latDivisions Número de divisiones latitudinales (longitud de la esfera).
         * @param longDivisions Número de divisiones longitudinales (circunferencia de la esfera).
         * @param radius Radio de la esfera.
         */
        Sphere(int latDivisions, int longDivisions, GLfloat radius);

        /**
         * @brief Destructor de la clase Sphere.
         *
         * El destructor elimina los recursos de OpenGL utilizados para la esfera, como los
         * buffers de vértices, índices y coordenadas de textura.
         */
        ~Sphere();

    private:
        int latitudeDivisions; ///< Número de divisiones latitudinales de la esfera.
        int longitudeDivisions; ///< Número de divisiones longitudinales de la esfera.
        GLfloat radius; ///< Radio de la esfera.

        /**
         * @brief Genera la geometría de la esfera.
         *
         * Este método calcula los vértices, coordenadas de textura y los índices para la
         * geometría de la esfera basándose en las divisiones latitudinales y longitudinales,
         * así como el radio de la esfera.
         */
        void generateGeometry();
    };
}
