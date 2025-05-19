#version 330 core

// Entrada desde el vertex shader
in vec2 texCoords;  // Coordenadas UV interpoladas

// Salida de color del fragmento
out vec4 fragment_color;

// Uniformes
uniform sampler2D textureSampler;  // Textura 2D para aplicar al objeto
uniform float transparency;        // Factor de transparencia (0.0 completamente transparente, 1.0 opaco)

void main()
{
	// Obtiene el color de la textura usando las coordenadas UV
	vec4 texColor = texture(textureSampler, texCoords);

	// Ajusta la componente alfa multiplicándola por el valor de transparencia
	fragment_color = vec4(texColor.rgb, texColor.a * transparency);
}
