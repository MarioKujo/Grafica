#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <iostream>
#include "../Headers/stb_image.hpp"

using namespace std;

class TextureLoader {
public:
    // Constructor y Destructor
    TextureLoader();
    ~TextureLoader();

    // Cargar textura desde archivo
    GLuint loadTexture(const string& filePath);

private:
    GLuint textureID;
};