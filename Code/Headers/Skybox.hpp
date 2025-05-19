/**
 * @file Skybox.hpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este archivo contiene la declaración de la clase Skybox, que es responsable de manejar un skybox
 * en una escena 3D. Un skybox es una estructura que envuelve toda la escena, normalmente usada para
 * representar el fondo del entorno, como el cielo, usando texturas de cubo.
 * Esta clase permite cargar las texturas del skybox, crear buffers de OpenGL y renderizar el skybox.
 */

#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <iostream>
#include <stb_image.h>
#include "GeometryGenerator.hpp"

namespace udit
{

     /**
      * @class Skybox
      * @brief Clase que representa un skybox en una escena 3D.
      *
      * La clase Skybox se encarga de la creación y renderizado de un cubo que representa el fondo de la escena.
      * Utiliza un conjunto de texturas (una para cada cara del cubo) y las mapea sobre un cubo 3D que se renderiza
      * como fondo estático. También maneja la carga de texturas, la creación de buffers y la configuración
      * de las propiedades necesarias para su renderizado en OpenGL.
      */
      class Skybox {
      public:
          /**
           * @brief Constructor de la clase Skybox.
           *
           * Este constructor recibe un vector de cadenas de texto que contiene las rutas de las imágenes que
           * se usarán para las seis caras del cubo del skybox.
           *
           * @param faces Rutas de las texturas para las caras del cubo.
           */
          Skybox();

          /**
           * @brief Destructor de la clase Skybox.
           *
           * Limpia los recursos asociados al skybox, incluyendo la liberación de la memoria usada para las
           * texturas y los buffers.
           */
          ~Skybox();

          /**
           * @brief Establece la textura del skybox.
           *
           * Este método recibe un identificador de textura y lo asigna al skybox.
           *
           * @param texture_id El identificador de la textura para el skybox.
           */
          void set_texture(GLuint texture_id);

          /**
           * @brief Obtiene el identificador de la textura del skybox.
           *
           * Este método retorna el identificador de la textura que se usa para el skybox.
           *
           * @return GLuint El identificador de la textura del skybox.
           */
          GLuint get_texture_id();

          /**
           * @brief Renderiza el skybox en la escena.
           *
           * Este método realiza el renderizado del cubo del skybox, usando la textura proporcionada para
           * las seis caras del cubo. Debe ser llamado en cada ciclo de renderizado.
           */
          void render();

      private:
          GLuint vao_id;      ///< Identificador del VAO (Vertex Array Object) del skybox.
          GLuint vbo_id;      ///< Identificador del VBO (Vertex Buffer Object) del skybox.
          GLuint texture_id;  ///< Identificador de la textura del skybox.

          /**
           * @brief Configura los buffers necesarios para renderizar el skybox.
           *
           * Este método crea los buffers de OpenGL (VAO, VBO) necesarios para almacenar la información
           * de los vértices que representan el cubo del skybox. Debe ser llamado una vez al inicio.
           */
          void setup_buffers();
      };
}
