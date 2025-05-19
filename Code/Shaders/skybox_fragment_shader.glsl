#version 330 core

// Coordenadas del cubemap interpoladas desde el vertex shader
in vec3 TexCoords;

// Color de salida del fragmento
out vec4 FragColor;

// Textura cúbica del skybox
uniform samplerCube skybox;

void main()
{
	// Muestra la textura cúbica usando la dirección del vector TexCoords
	FragColor = texture(skybox, TexCoords);
}
