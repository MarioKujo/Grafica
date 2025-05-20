// Este código es de dominio público
// Autor: andrmatgonros@gmail.com

#pragma once

#include "../Headers/Scene.hpp"

namespace udit
{

#pragma region constants
	// Constantes para definir alturas y escalas usadas en la escena
	constexpr float Scene::UFO_HEIGHT = 250.f;
	constexpr float Scene::COW_HEIGHT = -100.f;
	constexpr float Scene::CONE_HEIGHT = -400.f;
	constexpr float Scene::SCALE_SMALL = 0.1f;
	constexpr float Scene::SCALE_BIG = 10.f;
#pragma endregion

	// Constructor: inicializa la escena (cámara, mallas, shaders, texturas y nodos)
	Scene::Scene(unsigned width, unsigned height)
		: angle(0),
		camera(glm::vec3(0.f, 3.f, 8.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f)
	{
		// Inicializar recursos (mallas, modelos y shaders)
		initResources();

		// Construir objetos gráficos con referencias a recursos cargados
		initObjects();

		// Configurar jerarquía de nodos y transformaciones
		initSceneGraph();

		// Configuración OpenGL, texturas y demás
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glClearColor(.2f, .2f, .2f, 1.f);

		loadTextures();
		setTextures();

		defaultProgram->use();
		resize(width, height);
	}

	Scene::~Scene() = default;

	void Scene::initResources()
	{
		// Inicializar o cargar modelos/mallas
		plane = make_unique<Mesh>(generator.generatePlane(100, 100, 100, 100));
		cone = make_unique<Mesh>(generator.generateCone(30, 60, 10));
		ufo = make_unique<AssimpMesh>("../Objects/UFO.obj");
		cow = make_unique<AssimpMesh>("../Objects/cow.obj");

		// Inicializar shaders
		defaultProgram = make_unique<ShaderProgram>("../Code/Shaders/default_vertex_shader.glsl", "../Code/Shaders/default_fragment_shader.glsl");
		unlitProgram = make_unique<ShaderProgram>("../Code/Shaders/unlit_vertex_shader.glsl", "../Code/Shaders/unlit_fragment_shader.glsl");
		skyboxProgram = make_unique<ShaderProgram>("../Code/Shaders/skybox_vertex_shader.glsl", "../Code/Shaders/skybox_fragment_shader.glsl");
		heightmapProgram = make_unique<ShaderProgram>("../Code/Shaders/heightmap_vertex_shader.glsl", "../Code/Shaders/default_fragment_shader.glsl");

		// Crear skybox
		skybox = std::make_unique<Skybox>(generator.generateCube(), skyboxProgram.get());
	}

	void Scene::initObjects()
	{
		heightmapObj = std::make_unique<Object>(plane.get(), heightmapProgram.get());
		ufoObj = std::make_unique<Object>(ufo.get(), defaultProgram.get());
		cowObj = std::make_unique<Object>(cow.get(), defaultProgram.get());
		coneObj = std::make_unique<Object>(cone.get(), unlitProgram.get());
	}

	void Scene::initSceneGraph()
	{
		rootNode = std::make_shared<SceneNode>();
		rootNode->setTransform({ 25.f, 0.f, -35.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });

		ufoCowConeNode = std::make_shared<SceneNode>();
		ufoCowConeNode->setTransform({ 1.2f, 0.f, 1.14f }, { 0.f, 0.f, 0.f }, { SCALE_SMALL, SCALE_SMALL, SCALE_SMALL });

		ufoNode = std::make_shared<SceneNode>(ufoObj.get());
		ufoNode->setTransform({ 0.f, UFO_HEIGHT, 0.f }, { 90.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });

		cowNode = std::make_shared<SceneNode>(cowObj.get());
		cowNode->setTransform({ 0.f, COW_HEIGHT, 0.f }, { 0.f, 0.f, 0.f }, { SCALE_SMALL, SCALE_SMALL, SCALE_SMALL });

		coneNode = std::make_shared<SceneNode>(coneObj.get());
		coneNode->setTransform({ 0.f, CONE_HEIGHT, 0.f }, { 0.f, 0.f, 0.f }, { SCALE_BIG, SCALE_BIG, SCALE_BIG });

		ufoCowConeNode->addChild(ufoNode);
		ufoCowConeNode->addChild(cowNode);
		ufoCowConeNode->addChild(coneNode);

		heightmapNode = std::make_shared<SceneNode>(heightmapObj.get());
		heightmapNode->setTransform({ 0.f, -40.f, 0.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });

		rootNode->addChild(heightmapNode);
		rootNode->addChild(ufoCowConeNode);
	}

	// Carga las texturas desde archivos y asigna el skybox
	void Scene::loadTextures()
	{
		coneTextureID = textureLoader.loadTexture("../Textures/cone_texture.jpg");
		heightmapID = textureLoader.loadTexture("../Textures/heightmap.png");
		heightmapTextureID = textureLoader.loadTexture("../Textures/heightmap_texture.jpg");
		ufoTextureID = textureLoader.loadTexture("../Textures/UFO_texture.jpg");
		cowTextureID = textureLoader.loadTexture("../Textures/cow_texture.jpg");

		skyboxTextureID = textureLoader.loadCubemap({
			"../Textures/skybox-right-1.jpg", "../Textures/skybox-left.jpg", "../Textures/skybox-up.jpg",
			"../Textures/skybox-down.jpg", "../Textures/skybox-center.jpg", "../Textures/skybox-right-2.jpg"
			});

		skybox->setTexture(skyboxTextureID);
	}

	// Asocia los IDs de texturas cargadas a los objetos correspondientes
	void Scene::setTextures()
	{
		ufoObj->setTextureID(ufoTextureID);
		cowObj->setTextureID(cowTextureID);
		coneObj->setTextureID(coneTextureID);
		heightmapObj->setHeightmapTextureID(heightmapID);
		heightmapObj->setTextureID(heightmapTextureID);
	}

	// Actualiza la escena animando el ángulo (para efectos como rotación o flotación)
	void Scene::update()
	{
		angle = fmod(angle + 1.0f, 360.f);
	}

	// Renderiza la escena completa, incluyendo skybox, luces y objetos
	void Scene::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpieza de buffers

		glm::mat4 view_matrix = camera.get_view_matrix();   // Matriz de vista desde la cámara

		skybox->render(view_matrix, projection_matrix);      // Render del skybox
		lightSetup(view_matrix);                            // Configuración de luces

		// Animación senoidal de flotación y rotación del grupo ufo-vaca-cono
		float float_height = 2.0f;
		float y_offset = sin(angle / 100) * float_height;
		ufoCowConeNode->setTransform({ 5.f, y_offset, 5.f }, { 0.f, angle, 0.f }, { 0.1f, 0.1f, 0.1f });

		glm::mat4 identity = glm::mat4(1.f); // Matriz identidad para la raíz
		rootNode->render(identity, view_matrix, projection_matrix); // Render del grafo
	}

	void Scene::applyLightSettings(ShaderProgram& shader, const vector<glm::vec3>& dirsView,
		const vector<glm::vec3>& colors, const vector<float>& intensities)
	{
		for (int i = 0; i < dirsView.size(); ++i) {
			string idx = std::to_string(i);
			shader.setVec3("lightDirections[" + idx + "]", dirsView[i]);
			shader.setVec3("lightColors[" + idx + "]", colors[i]);
			shader.setFloat("lightIntensities[" + idx + "]", intensities[i]);
		}
	}

	// Envía información de iluminación a los shaders (dirección, color, intensidad, cámara)
	void Scene::lightSetup(glm::mat4& view_matrix)
	{
		vector<glm::vec3> lightDirsWorld = {
			glm::normalize(glm::vec3(-1.f, -1.f, -1.f)),
			glm::normalize(glm::vec3(1.f, 1.f, 1.f))
		};

		vector<glm::vec3> lightColors = {
			glm::vec3(1.0f),
			glm::vec3(1.0f)
		};

		vector<float> lightIntensities = { 1.0f, 1.0f };

		vector<glm::vec3> lightDirsView;
		for (const auto& dir : lightDirsWorld)
			lightDirsView.push_back(glm::mat3(view_matrix) * dir);

		heightmapProgram->use();
		applyLightSettings(*heightmapProgram, lightDirsView, lightColors, lightIntensities);

		defaultProgram->use();
		applyLightSettings(*defaultProgram, lightDirsView, lightColors, lightIntensities);
		defaultProgram->setVec3("viewPos", viewPos);
	}

	// Actualiza la proyección y el viewport al redimensionar la ventana
	void Scene::resize(unsigned width, unsigned height)
	{
		projection_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);

		defaultProgram->setMat4("projection_matrix", projection_matrix);

		glViewport(0, 0, width, height); // Ajuste del área de dibujo
	}

	// Establece una nueva cámara como activa
	void Scene::set_camera(Camera new_camera)
	{
		camera = new_camera;
	}

}
