// Este código es de dominio público
// andrmatgonros@gmail.com
#pragma once

#include "../Headers/Scene.hpp"
namespace udit
{
#pragma region Shaders
#pragma region vertex shader (lit)
	const string Scene::vertex_shader_code =
		"#version 330\n"
		"uniform mat4 model_view_matrix;"
		"uniform mat4 projection_matrix;"
		"layout (location = 0) in vec3 vertex_coordinates;"
		"layout (location = 1) in vec2 vertex_texCoords;"
		"layout (location = 2) in vec3 vertex_normal;"  // Normal del vértice
		"out vec2 texCoords;"
		"out vec3 fragNormal;"  // Enviar la normal al fragment shader
		"out vec3 fragPos;"     // Enviar la posición del vértice al fragment shader
		"void main()"
		"{"
		"   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
		"   texCoords = vertex_texCoords;"
		"   fragNormal = mat3(transpose(inverse(model_view_matrix))) * vertex_normal;" // Transformar la normal
		"   fragPos = vec3(model_view_matrix * vec4(vertex_coordinates, 1.0));"  // Obtener la posición
		"}";
#pragma endregion

#pragma region fragment shader (lit)
	// Código fuente del shader de fragmentos para la escena
	const string Scene::fragment_shader_code =
		"#version 330 core\n"
		"in vec2 texCoords;\n"
		"in vec3 fragNormal;\n"
		"in vec3 fragPos;\n"
		"out vec4 fragment_color;\n"
		"uniform sampler2D textureSampler;\n"
		"uniform float transparency;\n"

		"#define NUM_LIGHTS 3\n"
		"uniform vec3 lightDirections[NUM_LIGHTS];\n"
		"uniform vec3 lightColors[NUM_LIGHTS];\n"
		"uniform float lightIntensities[NUM_LIGHTS];"

		"void main()\n"
		"{\n"
		"    vec4 texColor = texture(textureSampler, texCoords);\n"
		"    vec3 norm = normalize(fragNormal);\n"
		"    vec3 totalDiffuse = vec3(0.0);\n"
		"    for (int i = 0; i < NUM_LIGHTS; ++i)\n"
		"    {\n"
		"        vec3 lightDir = normalize(-lightDirections[i]);\n"
		"        float diff = max(dot(norm, lightDir), 0.0);\n"
		"        totalDiffuse += diff * lightColors[i] * lightIntensities[i];\n"
		"    }\n"
		"    vec3 result = texColor.rgb * totalDiffuse;\n"
		"    fragment_color = vec4(result, texColor.a * transparency);\n"
		"}\n";

#pragma endregion

#pragma region vertex shader unlit
	const string Scene::vertex_shader_unlit_code =
		"#version 330 core\n"
		"uniform mat4 model_view_matrix;\n"
		"uniform mat4 projection_matrix;\n"
		"layout (location = 0) in vec3 vertex_coordinates;\n"
		"layout (location = 1) in vec2 vertex_texCoords;\n"
		"out vec2 texCoords;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);\n"
		"    texCoords = vertex_texCoords;\n"
		"}\n";
#pragma endregion

#pragma region fragment shader unlit
	const string Scene::fragment_shader_unlit_code =
		"#version 330 core\n"
		"in vec2 texCoords;\n"
		"out vec4 fragment_color;\n"
		"uniform sampler2D textureSampler;\n"
		"uniform float transparency;\n"
		"void main()\n"
		"{\n"
		"    vec4 texColor = texture(textureSampler, texCoords);\n"
		"    fragment_color = vec4(texColor.rgb, texColor.a * transparency);\n"
		"}\n";
#pragma endregion

#pragma region skybox vertex shader
	// Código fuente del shader de vértices para el skybox
	const string Scene::skybox_vertex_shader =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"out vec3 TexCoords;"
		"uniform mat4 projection;"
		"uniform mat4 view;"
		"void main()"
		"{"
		"   TexCoords = aPos;"
		"   vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);"
		"   gl_Position = pos.xyww;"
		"}";
#pragma endregion

#pragma region skybox fragment shader
	// Código fuente del shader de fragmentos para el skybox
	const string Scene::skybox_fragment_shader =
		"#version 330 core\n"
		"in vec3 TexCoords;"
		"out vec4 FragColor;"
		"uniform samplerCube skybox;"
		"void main()"
		"{"
		"   FragColor = texture(skybox, TexCoords);"
		"}";
#pragma endregion

#pragma region heightmap vertex shader
	const string Scene::heightmap_vertex_shader =
		"#version 330 core\n"
		"uniform mat4 model_view_matrix;"
		"uniform mat4 projection_matrix;"
		"uniform sampler2D heightmap;"  // Nueva textura de altura
		"uniform float height_scale;"   // Escala para la altura del heightmap
		"layout (location = 0) in vec3 vertex_coordinates;"
		"layout (location = 1) in vec2 vertex_texCoords;"
		"layout (location = 2) in vec3 vertex_normal;"
		"out vec2 texCoords;"
		"out vec3 fragNormal;"
		"out vec3 fragPos;"
		"void main()"
		"{"
		"   texCoords = vertex_texCoords;"
		"   float height = texture(heightmap, vertex_texCoords).r;"  // Obtener altura de la textura
		"   vec3 displaced_position = vertex_coordinates + vertex_normal * height * height_scale;" // Desplazamiento
		"   gl_Position = projection_matrix * model_view_matrix * vec4(displaced_position, 1.0);"
		"   fragNormal = mat3(transpose(inverse(model_view_matrix))) * vertex_normal;"
		"   fragPos = vec3(model_view_matrix * vec4(displaced_position, 1.0));"
		"}";
#pragma endregion

#pragma endregion
	// Constructor de la escena, inicializa objetos 3D, cámara, shaders y texturas
	Scene::Scene(unsigned width, unsigned height)
		: angle(0),
		camera(glm::vec3(0.f, 3.f, 8.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f),
		heightmap(100, 100, 100, 100), cone(10, 20, 40), ufo("../Objects/UFO.obj"),
		skybox()
#pragma region Constructor
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glClearColor(.2f, .2f, .2f, 1.f);

		initializeShaders();
		initializeUniformLocations();
		initializeLightSettings();
		loadTextures();
		resize(width, height);
	}
#pragma endregion

	void Scene::initializeShaders() {
		program_id = shaderProgram.compile_shaders(vertex_shader_code, fragment_shader_code);
		unlit_program_id = shaderProgram.compile_shaders(vertex_shader_unlit_code, fragment_shader_unlit_code);
		glUseProgram(program_id);
		skybox_program_id = shaderProgram.compile_shaders(skybox_vertex_shader, skybox_fragment_shader);
		heightmap_program_id = shaderProgram.compile_shaders(heightmap_vertex_shader, fragment_shader_code);
	}

	void Scene::initializeUniformLocations() {
		model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
		projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");

		skybox_model_view_matrix_id = glGetUniformLocation(skybox_program_id, "view");
		skybox_projection_matrix_id = glGetUniformLocation(skybox_program_id, "projection");

		heightmap_model_view_matrix_id = glGetUniformLocation(heightmap_program_id, "model_view_matrix");
		heightmap_projection_matrix_id = glGetUniformLocation(heightmap_program_id, "projection_matrix");

		lightPos_id = glGetUniformLocation(program_id, "lightPos");
		lightColor_id = glGetUniformLocation(program_id, "lightColor");
		viewPos_id = glGetUniformLocation(program_id, "viewPos");
	}

	void Scene::initializeLightSettings() {
		lightPos = glm::vec3(10.0f, 10.0f, 10.0f);  // Posición de la luz
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f);  // Color blanco para la luz
		viewPos = glm::vec3(0.0f, 0.0f, 8.0f);    // Posición de la cámara
	}

	void Scene::loadTextures() {
		coneTextureID = textureLoader.loadTexture("../Textures/cone_texture.jpg");
		heightmapID = textureLoader.loadTexture("../Textures/heightmap.png");
		heightmapTextureID = textureLoader.loadTexture("../Textures/heightmap_texture.jpg");
		ufoTextureID = textureLoader.loadTexture("../Textures/UFO_texture.jpg");
		skyboxTextureID = textureLoader.loadCubemap({
			"../Textures/skybox-right-1.jpg", "../Textures/skybox-left.jpg", "../Textures/skybox-up.jpg",
			"../Textures/skybox-down.jpg", "../Textures/skybox-center.jpg", "../Textures/skybox-right-2.jpg" });
		skybox.set_texture(skyboxTextureID);
	}

	// Actualiza la escena (cámara y rotación de objetos)
	void Scene::update()
	{
		angle += 0.01f;
	}

	// Renderiza todos los objetos en la escena
	void Scene::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 view_matrix = camera.get_view_matrix();
		renderSkybox(view_matrix);

		glUseProgram(program_id);

		lightSetup(view_matrix);

		renderUFO(view_matrix);

		renderHeightmap(view_matrix);

		renderCone(view_matrix);
	}

	void Scene::renderSkybox(glm::mat4& view_matrix)
	{
		glUseProgram(skybox_program_id);

		glUniformMatrix4fv(skybox_model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(skybox_projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.get_texture_id());
		skybox.render();
	}

	void Scene::lightSetup(glm::mat4& view_matrix)
	{
		// Definimos múltiples luces en coordenadas del mundo
		vector<glm::vec3> lightDirsWorld = {
			glm::normalize(glm::vec3(-1.f, -1.f, -1.f)),
			glm::normalize(glm::vec3(1.f, 1.f, 1.f))
		};

		vector<glm::vec3> lightColors = {
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		};

		// Transformar direcciones al sistema de vista
		vector<glm::vec3> lightDirsView;
		for (const auto& dir : lightDirsWorld) {
			lightDirsView.push_back(glm::mat3(view_matrix) * dir);
		}

		vector<float> lightIntensities = {
			1.0f,
			1.0f
		};
		glUseProgram(heightmap_program_id);
		glUniform3fv(glGetUniformLocation(heightmap_program_id, "lightDirections"), (GLsizei)lightDirsView.size(), glm::value_ptr(lightDirsView[0]));
		glUniform3fv(glGetUniformLocation(heightmap_program_id, "lightColors"), (GLsizei)lightColors.size(), glm::value_ptr(lightColors[0]));
		glUniform1fv(glGetUniformLocation(heightmap_program_id, "lightIntensities"), (GLsizei)lightIntensities.size(), &lightIntensities[0]);
		glUniform1f(glGetUniformLocation(heightmap_program_id, "transparency"), 1.0f);

		glUseProgram(program_id);
		glUniform3fv(glGetUniformLocation(program_id, "lightDirections"), (GLsizei)lightDirsView.size(), glm::value_ptr(lightDirsView[0]));
		glUniform3fv(glGetUniformLocation(program_id, "lightColors"), (GLsizei)lightColors.size(), glm::value_ptr(lightColors[0]));
		glUniform1fv(glGetUniformLocation(program_id, "lightIntensities"), (GLsizei)lightIntensities.size(), &lightIntensities[0]);
		glUniform3fv(viewPos_id, 1, glm::value_ptr(viewPos));

	}

	void Scene::renderUFO(glm::mat4& view_matrix)
	{

		glm::mat4 ufo_matrix(1.0f);
		ufo_matrix = glm::translate(ufo_matrix, glm::vec3(30.f, 45.f, -40.f));
		ufo_matrix = glm::rotate(ufo_matrix, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
		ufo_matrix = glm::scale(ufo_matrix, glm::vec3(0.1f));
		ufo_matrix = glm::rotate(ufo_matrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 ufo_view_matrix = view_matrix * ufo_matrix;

		glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(ufo_view_matrix));
		glUniform1f(glGetUniformLocation(program_id, "transparency"), 1.0f);
		glBindTexture(GL_TEXTURE_2D, ufoTextureID);

		ufo.render();
	}

	void Scene::renderHeightmap(glm::mat4& view_matrix)
	{
		glUseProgram(heightmap_program_id);

		glm::mat4 heightmap_matrix(1);
		heightmap_matrix = glm::translate(heightmap_matrix, glm::vec3(25.f, -16.f, -35.f));
		glm::mat4 heightmap_view_matrix = view_matrix * heightmap_matrix;

		glUniformMatrix4fv(heightmap_model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(heightmap_view_matrix));
		glUniformMatrix4fv(heightmap_projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(heightmap_program_id, "textureSampler"), 0); // sampler0
		glBindTexture(GL_TEXTURE_2D, heightmapTextureID);

		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(heightmap_program_id, "heightmap"), 1); // sampler1
		glBindTexture(GL_TEXTURE_2D, heightmapID);

		float height_scale = 25.0f;
		glUniform1f(glGetUniformLocation(heightmap_program_id, "height_scale"), height_scale);
		// Renderiza el plano (terreno)
		heightmap.render();
	}

	void Scene::renderCone(glm::mat4& view_matrix)
	{
		glUseProgram(unlit_program_id);
		glm::mat4 cone_matrix(1.0f);
		cone_matrix = glm::translate(cone_matrix, glm::vec3(30.f, -3.4f, -40.f));
		cone_matrix = glm::rotate(cone_matrix, angle, glm::vec3(0.f, -1.f, 0.f));
		glm::mat4 cone_view_matrix = view_matrix * cone_matrix;

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(unlit_program_id, "textureSampler"), 0);
		glBindTexture(GL_TEXTURE_2D, coneTextureID);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLint transparency = glGetUniformLocation(unlit_program_id, "transparency");
		glUniform1f(transparency, 0.5f);

		glUniformMatrix4fv(glGetUniformLocation(unlit_program_id, "model_view_matrix"), 1, GL_FALSE, glm::value_ptr(cone_view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(unlit_program_id, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
		cone.render();

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

		glUseProgram(program_id);
	}

	// Ajusta el tamaño de la ventana y la proyección
	void Scene::resize(unsigned width, unsigned height)
	{
		projection_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);
		glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		glViewport(0, 0, width, height);
	}

	void Scene::set_camera(Camera new_camera)
	{
		camera = new_camera;
	}


}
