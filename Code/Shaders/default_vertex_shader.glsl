#version 330

// Matrices uniformes que se pasan desde la aplicación
uniform mat4 model_view_matrix;    // Matriz de transformación del modelo a vista
uniform mat4 projection_matrix;    // Matriz de proyección

// Atributos de entrada del vértice
layout (location = 0) in vec3 vertex_coordinates;  // Coordenadas del vértice
layout (location = 1) in vec2 vertex_texCoords;    // Coordenadas de textura del vértice
layout (location = 2) in vec3 vertex_normal;       // Normal del vértice

// Salidas hacia el fragment shader
out vec2 texCoords;       // Coordenadas de textura para el fragment shader
out vec3 fragNormal;      // Normal transformada para iluminación
out vec3 fragPos;         // Posición del fragmento en espacio de vista

void main()
{
	// Transformación completa del vértice a espacio de clip
	gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);

	// Pasamos directamente las coordenadas de textura
	texCoords = vertex_texCoords;

	// Transformación correcta de normales al espacio de vista
	fragNormal = mat3(transpose(inverse(model_view_matrix))) * vertex_normal;

	// Calculamos la posición del vértice en espacio de vista
	fragPos = vec3(model_view_matrix * vec4(vertex_coordinates, 1.0));
}
