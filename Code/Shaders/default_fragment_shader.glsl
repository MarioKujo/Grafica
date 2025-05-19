#version 330 core

// Entradas desde el vertex shader
in vec2 texCoords;      // Coordenadas de textura interpoladas
in vec3 fragNormal;     // Normal interpolada en espacio de vista
in vec3 fragPos;        // Posición del fragmento en espacio de vista

// Salida del color final del fragmento
out vec4 fragment_color;

// Textura del material
uniform sampler2D textureSampler;

// Definición del número de luces a usar
#define NUM_LIGHTS 3

// Parámetros de las luces
uniform vec3 lightDirections[NUM_LIGHTS];     // Direcciones de las luces
uniform vec3 lightColors[NUM_LIGHTS];         // Colores de las luces
uniform float lightIntensities[NUM_LIGHTS];   // Intensidades de cada luz

void main()
{
	// Color base del fragmento tomado de la textura
	vec4 texColor = texture(textureSampler, texCoords);

	// Normalizamos la normal para los cálculos de iluminación
	vec3 norm = normalize(fragNormal);

	// Inicializamos la suma de la iluminación difusa
	vec3 totalDiffuse = vec3(0.0);

	// Cálculo de iluminación difusa para cada luz
	for (int i = 0; i < NUM_LIGHTS; ++i)
	{
		vec3 lightDir = normalize(-lightDirections[i]);  // Invertimos dirección (de la luz hacia el fragmento)
		float diff = max(dot(norm, lightDir), 0.0);      // Producto punto (ángulo entre normal y dirección de luz)
		totalDiffuse += diff * lightColors[i] * lightIntensities[i];  // Acumulamos la contribución de cada luz
	}

	// Combinamos la iluminación difusa con el color base de la textura
	vec3 result = texColor.rgb * totalDiffuse;

	// Asignamos el color final del fragmento (manteniendo la componente alfa de la textura)
	fragment_color = vec4(result, texColor.a);
}
