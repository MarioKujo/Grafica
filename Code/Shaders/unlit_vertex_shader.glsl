#version 330 core
uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
layout (location = 0) in vec3 vertex_coordinates;
layout (location = 1) in vec2 vertex_texCoords;
out vec2 texCoords;
void main()
{
	gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);
	texCoords = vertex_texCoords;
}