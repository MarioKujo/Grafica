		#version 330 core
		uniform mat4 model_view_matrix;
		uniform mat4 projection_matrix;
		uniform sampler2D heightmap;  // Textura con mapa de alturas
		uniform float height_scale;   // Factor de escala para el desplazamiento en altura
		layout (location = 0) in vec3 vertex_coordinates;
		layout (location = 1) in vec2 vertex_texCoords;
		layout (location = 2) in vec3 vertex_normal;
		out vec2 texCoords;
		out vec3 fragNormal;
		out vec3 fragPos;
		void main()
		{
		   texCoords = vertex_texCoords;
		   float height = texture(heightmap, vertex_texCoords).r;  // Leer altura del mapa
		   vec3 displaced_position = vertex_coordinates + vertex_normal * height * height_scale; // Desplazar vértice
		   gl_Position = projection_matrix * model_view_matrix * vec4(displaced_position, 1.0);
		   fragNormal = mat3(transpose(inverse(model_view_matrix))) * vertex_normal;
		   fragPos = vec3(model_view_matrix * vec4(displaced_position, 1.0));
		}