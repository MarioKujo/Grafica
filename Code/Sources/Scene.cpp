// Este código es de dominio público
// andrmatgonros@gmail.com
#pragma once

#include "../Headers/Scene.hpp"
namespace udit
{
#pragma region constants
	const float Scene::UFO_HEIGHT = 250.f;
	const float Scene::COW_HEIGHT = -100.f;
	const float Scene::CONE_HEIGHT = -400.f;
	const float Scene::SCALE_SMALL = 0.1f;
	const float Scene::SCALE_BIG = 10.f;
#pragma endregion
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
		"    fragment_color = vec4(result, texColor.a);\n"
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
		plane(generator.generatePlane(100, 100, 100, 100)),
		cone(generator.generateCone(30, 60, 10)),
		ufo("../Objects/UFO.obj"),
		cow("../Objects/cow.obj"),
		
		defaultProgram(vertex_shader_code, fragment_shader_code),
		unlitProgram(vertex_shader_unlit_code, fragment_shader_unlit_code),
		skyboxProgram(skybox_vertex_shader, skybox_fragment_shader),
		heightmapProgram(heightmap_vertex_shader, fragment_shader_code),

		skybox(generator.generateCube(), &skyboxProgram),
		heightmapObj(&plane, &heightmapProgram),
		ufoObj(&ufo, &defaultProgram),
		cowObj(&cow, &defaultProgram),
		coneObj(&cone, &unlitProgram)

#pragma region Constructor
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glClearColor(.2f, .2f, .2f, 1.f);

		loadTextures();
		
		setGraph();

		defaultProgram.use();
		resize(width, height);
	}
#pragma endregion

	void Scene::setGraph()
	{
		rootNode = make_shared<SceneNode>();
		rootNode->setTransform({ 25.f, 0.f, -35.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f , 1.f });

		ufoCowConeNode = make_shared<SceneNode>();
		ufoCowConeNode->setTransform({1.2f, 0.f, 1.14f}, {0.f, 0.f, 0.f}, {SCALE_SMALL, SCALE_SMALL, SCALE_SMALL });

		ufoNode = make_shared<SceneNode>(&ufoObj);
		ufoNode->setTransform({ 0.f, UFO_HEIGHT, 0.f }, { 90.f, 0.f, 0.f}, { 1.f, 1.f, 1.f });

		cowNode = make_shared<SceneNode>(&cowObj);
		cowNode->setTransform({ 0.f, COW_HEIGHT, 0.f }, { 0.f, 0.f, 0.f}, { SCALE_SMALL, SCALE_SMALL, SCALE_SMALL });

		coneNode = make_shared<SceneNode>(&coneObj);
		coneNode->setTransform({0.f, CONE_HEIGHT, 0.f}, {0.f, 0.f, 0.f}, { SCALE_BIG, SCALE_BIG, SCALE_BIG });

		ufoCowConeNode->addChild(ufoNode);
		ufoCowConeNode->addChild(cowNode);
		ufoCowConeNode->addChild(coneNode);

		heightmapNode = make_shared<SceneNode>(&heightmapObj);
		heightmapNode->setTransform({0.f, -40.f, 0.f}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f});

		// Añadir el OVNI a la raíz
		rootNode->addChild(heightmapNode);
		rootNode->addChild(ufoCowConeNode);
	}

	void Scene::loadTextures() {
		coneTextureID = textureLoader.loadTexture("../Textures/cone_texture.jpg");
		heightmapID = textureLoader.loadTexture("../Textures/heightmap.png");
		heightmapTextureID = textureLoader.loadTexture("../Textures/heightmap_texture.jpg");
		ufoTextureID = textureLoader.loadTexture("../Textures/UFO_texture.jpg");
		cowTextureID = textureLoader.loadTexture("../Textures/cow_texture.jpg");
		skyboxTextureID = textureLoader.loadCubemap({
			"../Textures/skybox-right-1.jpg", "../Textures/skybox-left.jpg", "../Textures/skybox-up.jpg",
			"../Textures/skybox-down.jpg", "../Textures/skybox-center.jpg", "../Textures/skybox-right-2.jpg" });
		skybox.setTexture(skyboxTextureID);

		setTextures();
	}

	void Scene::setTextures()
	{
		ufoObj.setTextureID(ufoTextureID);

		cowObj.setTextureID(cowTextureID);

		coneObj.setTextureID(coneTextureID);

		heightmapObj.setHeightmapTextureID(heightmapID);
		heightmapObj.setTextureID(heightmapTextureID);
	}

	// Actualiza la escena (cámara y rotación de objetos)
	void Scene::update()
	{
		angle += 1.0f;
	}

	// Renderiza todos los objetos en la escena
	void Scene::render() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 view_matrix = camera.get_view_matrix();

		skybox.render(view_matrix, projection_matrix);

		lightSetup(view_matrix);

		float float_height = 2.0f; // amplitud de flotación
		float y_offset = sin(angle / 100) * float_height;
		glm::mat4 identity = glm::mat4(1.f);
		ufoCowConeNode->setTransform({ 5.f, y_offset, 5.f }, { 0.f, angle, 0.f }, { 0.1f, 0.1f, 0.1f });
		rootNode->render(identity, view_matrix, projection_matrix);
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

		heightmapProgram.use();

		for (int i = 0; i < 2; ++i) {
			string index = std::to_string(i);
			heightmapProgram.setVec3("lightDirections[" + index + "]", lightDirsView[i]);
			heightmapProgram.setVec3("lightColors[" + index + "]", lightColors[i]);
			heightmapProgram.setFloat("lightIntensities[" + index + "]", lightIntensities[i]);
		}


		defaultProgram.use();

		for (int i = 0; i < 2; ++i) {
			string index = std::to_string(i);
			defaultProgram.setVec3("lightDirections[" + index + "]", lightDirsView[i]);
			defaultProgram.setVec3("lightColors[" + index + "]", lightColors[i]);
			defaultProgram.setFloat("lightIntensities[" + index + "]", lightIntensities[i]);
		}
		defaultProgram.setVec3("viewPos", viewPos);
	}

	// Ajusta el tamaño de la ventana y la proyección
	void Scene::resize(unsigned width, unsigned height)
	{
		projection_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);
		defaultProgram.setMat4("projection_matrix", projection_matrix);
		glViewport(0, 0, width, height);
	}

	void Scene::set_camera(Camera new_camera)
	{
		camera = new_camera;
	}

}
