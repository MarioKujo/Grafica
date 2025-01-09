#include "../Headers/TextureLoader.hpp"
TextureLoader::TextureLoader() : textureID(0) {}

TextureLoader::~TextureLoader() {
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
}

GLuint TextureLoader::loadTexture(const string& filePath) {
    // Cargar la imagen usando stb_image
    int width, height, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if (!data) {
        cerr << "Error al cargar la textura: " << filePath << endl;
        return 0;
    }

    // Generar textura en OpenGL
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Establecer parámetros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Determinar formato según el número de canales de la imagen
    GLenum format = GL_RGB;
    if (channels == 4) {
        format = GL_RGBA;
    }

    // Cargar los datos de la imagen a la textura
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Liberar la memoria de la imagen
    stbi_image_free(data);

    return textureID;
}