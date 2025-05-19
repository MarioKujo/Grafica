#version 330 core

// Matrices de transformación
uniform mat4 model_view_matrix;     // Matriz que transforma de modelo a vista
uniform mat4 projection_matrix;     // Matriz de proyección (vista a espacio de clip)

// Uniformes específicos del mapa de alturas
uniform sampler2D heightmap;        // Textura que contiene el mapa de alturas (escala de grises)
uniform float height_scale;         // Factor de escala para ajustar cuánto se eleva el vértice

// Atributos de entrada del vértice
layout (location = 0) in vec3 vertex_coordinates;   // Coordenadas originales del vértice
layout (location = 1) in vec2 vertex_texCoords;     // Coordenadas de textura (UV)
layout (location = 2) in vec3 vertex_normal;        // Normal original del vértice

// Salidas hacia el fragment shader
out vec2 texCoords;     // Coordenadas de textura interpoladas
out vec3 fragNormal;    // Normal transformada
out vec3 fragPos;       // Posición del vértice desplazado (en espacio de vista)

void main()
{
   // Pasamos las coordenadas de textura directamente al fragment shader
   texCoords = vertex_texCoords;

   // Se obtiene la altura desde la textura del mapa de alturas (usando solo el canal rojo)
   float height = texture(heightmap, vertex_texCoords).r;

   // Se calcula la nueva posición del vértice desplazándolo a lo largo de su normal
   vec3 displaced_position = vertex_coordinates + vertex_normal * height * height_scale;

   // Transformamos la posición desplazada al espacio de clip
   gl_Position = projection_matrix * model_view_matrix * vec4(displaced_position, 1.0);

   // Transformamos la normal al espacio de vista correctamente (con matriz inversa transpuesta)
   fragNormal = mat3(transpose(inverse(model_view_matrix))) * vertex_normal;

   // Calculamos la posición del vértice desplazado en el espacio de vista
   fragPos = vec3(model_view_matrix * vec4(displaced_position, 1.0));
}
