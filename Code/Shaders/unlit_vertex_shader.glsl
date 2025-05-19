#version 330 core

// Matrices uniformes para transformar los vértices
uniform mat4 model_view_matrix;   // Transforma del espacio del modelo al espacio de la cámara
uniform mat4 projection_matrix;   // Proyección de cámara (perspectiva u ortográfica)

// Entrada del vértice
layout (location = 0) in vec3 vertex_coordinates;  // Posición del vértice
layout (location = 1) in vec2 vertex_texCoords;    // Coordenadas de textura (UV)

// Salida hacia el fragment shader
out vec2 texCoords;  // Coordenadas de textura interpoladas

void main()
{
	// Calcula la posición final del vértice en espacio de clip
	gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);

	// Pasa las coordenadas de textura al fragment shader
	texCoords = vertex_texCoords;
}
