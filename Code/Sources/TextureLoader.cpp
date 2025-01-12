/**
 * @file TextureLoader.cpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este archivo contiene la implementación de la clase `TextureLoader`, que facilita la carga
 * de texturas 2D y cubemaps en OpenGL.
 */

#include "../Headers/TextureLoader.hpp"

 /**
  * @brief Constructor de la clase TextureLoader.
  *
  * Inicializa el ID de la textura en 0. Este constructor se utiliza para crear un objeto
  * de la clase `TextureLoader`, el cual no contiene texturas asignadas inicialmente.
  */
TextureLoader::TextureLoader() : textureID(0) {}

/**
 * @brief Destructor de la clase TextureLoader.
 *
 * Si el ID de la textura es válido (no es 0), se elimina la textura cargada para liberar recursos.
 */
TextureLoader::~TextureLoader() {
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
}

/**
 * @brief Carga una textura 2D desde un archivo.
 *
 * Este método carga una imagen desde un archivo y la utiliza para crear una textura 2D en OpenGL.
 * Además, genera los mipmaps correspondientes para mejorar la calidad de la textura a medida que
 * se aleja de la cámara.
 *
 * @param filePath Ruta del archivo de la imagen que se desea cargar como textura.
 * @return GLuint ID de la textura cargada. Si hay un error al cargar la imagen, devuelve 0.
 */
GLuint TextureLoader::loadTexture(const string& filePath) {

    int width, height, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if (!data) {
        cerr << "Error al cargar la textura: " << filePath << endl;
        return 0;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Establecer parámetros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format = GL_RGB;
    if (channels == 4) {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return textureID;
}

/**
 * @brief Carga un cubemap desde una lista de imágenes.
 *
 * Este método carga una serie de imágenes que representan las caras de un cubemap.
 * Los cubemaps son utilizados para efectos como el reflejo o el fondo del entorno 3D.
 *
 * @param faces Lista de rutas de los archivos de imagen que representan las caras del cubemap.
 * @return GLuint ID del cubemap cargado. Si hay un error al cargar alguna imagen, devuelve 0.
 */
GLuint TextureLoader::loadCubemap(const vector<string>& faces) {
    GLuint cubemapID;
    glGenTextures(1, &cubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

    int width, height, channels;
    for (GLuint i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (!data) {
            cerr << "Error al cargar la textura del cubemap: " << faces[i] << endl;
            return 0;
        }

        GLenum format = GL_RGB;
        if (channels == 4) {
            format = GL_RGBA;
        }

        // Asigna cada imagen a una cara del cubemap
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    // Establecer parámetros del cubemap
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return cubemapID;
}
