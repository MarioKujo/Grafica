#version 330 core

// Atributo de entrada: posición del vértice del cubo
layout (location = 0) in vec3 aPos;

// Salida al fragment shader: dirección de la textura cúbica
out vec3 TexCoords;

// Matrices de proyección y vista
uniform mat4 projection;
uniform mat4 view;

void main()
{
	// Usamos la posición del vértice como dirección para el muestreo de la textura del skybox
	TexCoords = aPos;

	// Convertimos la matriz de vista a una versión 3x3 (sin traslación) y la convertimos a 4x4
	// Esto evita que el skybox se traslade con la cámara (sólo rota)
	vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);

	// Usamos .xyww en lugar de .xyzw para forzar una profundidad constante en el z-buffer
	gl_Position = pos.xyww;
}
