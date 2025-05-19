// Este código es de dominio público
// andrmatgonros@gmail.com

#include "../Headers/TextureLoader.hpp"

namespace udit
{
    TextureLoader::TextureLoader() : textureID(0) {}

    TextureLoader::~TextureLoader() {
        // Si hay una textura 2D cargada, eliminarla para liberar recursos
        if (textureID) {
            glDeleteTextures(1, &textureID);
        }
    }

    GLuint TextureLoader::loadTexture(const string& filePath) {
        int width, height, channels;

        // Cargar la imagen desde el archivo
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
        if (!data) {
            cerr << "Error al cargar la textura: " << filePath << endl;
            return 0;
        }

        // Generar un identificador de textura y enlazarlo como textura 2D
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Configurar parámetros de filtrado y repetición
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Determinar el formato de la imagen (RGB o RGBA)
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

        // Cargar los datos de la imagen en OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Liberar los datos de imagen cargados
        stbi_image_free(data);

        return textureID;
    }

    GLuint TextureLoader::loadCubemap(const vector<string>& faces) {
        GLuint cubemapID;

        // Generar e identificar el cubemap en OpenGL
        glGenTextures(1, &cubemapID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

        int width, height, channels;

        // Cargar cada cara del cubemap
        for (GLuint i = 0; i < faces.size(); ++i) {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
            if (!data) {
                cerr << "Error al cargar la textura del cubemap: " << faces[i] << endl;
                return 0;
            }

            // Determinar el formato (RGB o RGBA)
            GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

            // Asignar la imagen a la cara correspondiente del cubemap
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                format,
                width,
                height,
                0,
                format,
                GL_UNSIGNED_BYTE,
                data
            );

            // Liberar los datos cargados
            stbi_image_free(data);
        }

        // Configurar los parámetros del cubemap
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return cubemapID;
    }
}
