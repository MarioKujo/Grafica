/* @file TextureLoader.cpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 */
#include "../Headers/TextureLoader.hpp"

TextureLoader::TextureLoader() : textureID(0) {}

TextureLoader::~TextureLoader() {
    // Si hay una textura cargada, eliminarla para liberar recursos
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
}

GLuint TextureLoader::loadTexture(const string& filePath) {
    int width, height, channels;

    // Carga la imagen desde el archivo
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        cerr << "Error al cargar la textura: " << filePath << endl;
        return 0;
    }

    // Genera la textura 2D en OpenGL
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Establece los parámetros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Define el formato de la textura según el número de canales
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    // Carga la textura en OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Libera la memoria de la imagen cargada
    stbi_image_free(data);

    return textureID;
}

GLuint TextureLoader::loadCubemap(const vector<string>& faces) {
    GLuint cubemapID;

    // Genera el cubemap en OpenGL
    glGenTextures(1, &cubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

    int width, height, channels;

    // Carga las imágenes para cada cara del cubemap
    for (GLuint i = 0; i < faces.size(); ++i) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (!data) {
            cerr << "Error al cargar la textura del cubemap: " << faces[i] << endl;
            return 0;
        }

        // Define el formato de la textura según el número de canales
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

        // Asigna la imagen a una cara del cubemap
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Libera la memoria de la imagen cargada
        stbi_image_free(data);
    }

    // Establece los parámetros del cubemap
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return cubemapID;
}
