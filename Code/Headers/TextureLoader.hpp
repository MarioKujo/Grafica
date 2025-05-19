#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

using namespace std;
namespace udit
{
    class TextureLoader
    {
    public:
        TextureLoader();

        ~TextureLoader();

        GLuint loadTexture(const string& filePath);

        GLuint loadCubemap(const vector<string>& faces);

    private:
        GLuint textureID;
    };
}
