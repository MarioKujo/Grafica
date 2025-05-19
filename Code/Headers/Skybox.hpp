#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <iostream>
#include <stb_image.h>
#include "Mesh.hpp"
#include "TextureLoader.hpp"

namespace udit
{
      class Skybox
      {
      public:

          Skybox(MeshData mesh, ShaderProgram* shader);

          ~Skybox();

          void setTexture(GLuint textureID);

          GLuint getTextureID();

          void render(const glm::mat4& view, const glm::mat4& projection);

      private:
          Mesh mesh;
          ShaderProgram* shader;

          GLuint textureID;  ///< Identificador de la textura del skybox.
      };
}
