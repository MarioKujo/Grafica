#version 330
uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
layout (location = 0) in vec3 vertex_coordinates;
layout (location = 1) in vec2 vertex_texCoords;
layout (location = 2) in vec3 vertex_normal;  // Normal del vértice
out vec2 texCoords;
out vec3 fragNormal;  // Normal para el fragment shader
out vec3 fragPos;     // Posición del fragmento para cálculos de luz
void main()
{
	gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);
	texCoords = vertex_texCoords;
	fragNormal = mat3(transpose(inverse(model_view_matrix))) * vertex_normal; // Transformación correcta de normales
	fragPos = vec3(model_view_matrix * vec4(vertex_coordinates, 1.0));
}