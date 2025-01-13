/*@file Heightmap.hpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Definición de la clase Heightmap para manejar mapas de altura en la generación de terrenos.
 *
 * Esta clase es responsable de cargar un mapa de altura desde una imagen, generar la malla de vértices
 * que representa el terreno y renderizarla utilizando OpenGL. El mapa de altura es una imagen de escala
 * de grises donde cada píxel define la elevación del terreno en un punto específico.
 */

#pragma once

#include <vector>
#include <string>
#include <glm.hpp>
#include <glad/glad.h>
namespace udit
{

     /**
      * @class Heightmap
      * @brief Clase que representa un mapa de altura, utilizado para generar un terreno.
      *
      * Esta clase maneja la carga de un mapa de altura, la generación de la malla correspondiente
      * en función de los datos de altura, y la renderización del terreno usando OpenGL.
      * El mapa de altura es una imagen de escala de grises que se utiliza para determinar las
      * elevaciones del terreno en cada punto de la malla.
      */
      class Heightmap {
      public:
          /**
           * @brief Constructor de la clase Heightmap.
           *
           * Este constructor carga los datos del mapa de altura desde un archivo de imagen,
           * genera la malla de vértices basada en esos datos y carga la textura del mapa de altura.
           *
           * @param heightmapPath Ruta del archivo de imagen que contiene los datos del mapa de altura.
           * @param width Ancho del terreno en unidades del mundo.
           * @param height Altura del terreno en unidades del mundo.
           * @param maxHeight Altura máxima en el mapa de altura.
           */
          Heightmap(const std::string& heightmapPath, float width, float height, float maxHeight);

          /**
           * @brief Destructor de la clase Heightmap.
           *
           * Este destructor se encarga de liberar los recursos asociados con la textura y los buffers de OpenGL.
           */
          ~Heightmap();

          /**
           * @brief Renderiza el mapa de altura en la pantalla.
           *
           * Este método utiliza los buffers de OpenGL para dibujar el mapa de altura en la pantalla.
           */
          void render() const;

          /**
           * @brief Obtiene el ID de la textura del mapa de altura.
           *
           * Este método devuelve el identificador de la textura de OpenGL utilizada para el mapa de altura.
           *
           * @return GLuint El identificador de la textura.
           */
          GLuint getTextureID() const { return textureID; }

      private:
          GLuint vao; ///< Identificador del Vertex Array Object.
          GLuint vbo; ///< Identificador del Vertex Buffer Object.
          GLuint ebo; ///< Identificador del Element Buffer Object.
          GLuint textureID; ///< Identificador de la textura del mapa de altura.
          unsigned int numIndices; ///< Número de índices para el dibujo del terreno.

          /**
           * @brief Genera la malla de vértices a partir de los datos de altura.
           *
           * Este método crea los vértices y los índices de la malla que representa el terreno,
           * utilizando los datos del mapa de altura cargado y las dimensiones proporcionadas.
           *
           * @param heightData Datos de altura extraídos del archivo del mapa de altura.
           * @param width Ancho del terreno en unidades del mundo.
           * @param height Altura del terreno en unidades del mundo.
           * @param maxHeight Altura máxima en el mapa de altura.
           */
          void generateMesh(const std::vector<float>& heightData, int width, int height, float maxHeight);

          /**
           * @brief Carga los datos del mapa de altura desde un archivo de imagen.
           *
           * Este método carga los datos de altura desde un archivo de imagen, donde cada píxel
           * representa la elevación en un punto del terreno. La imagen debe estar en formato
           * escala de grises, y se utiliza para determinar la altura de cada vértice en la malla.
           *
           * @param heightmapPath Ruta del archivo de imagen del mapa de altura.
           * @param width Variable que almacenará el ancho del mapa de altura cargado.
           * @param height Variable que almacenará la altura del mapa de altura cargado.
           *
           * @return std::vector<float> Un vector con los datos de altura, uno por cada píxel de la imagen.
           */
          std::vector<float> loadHeightData(const std::string& heightmapPath, int& width, int& height);
      };
}
