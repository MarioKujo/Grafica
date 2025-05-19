// Este código es de dominio público
// Autor: andrmatgonros@gmail.com

#pragma once

#include "../Headers/Scene.hpp"

namespace udit
{

#pragma region constants
	// Constantes para definir alturas y escalas usadas en la escena
	const float Scene::UFO_HEIGHT = 250.f;
	const float Scene::COW_HEIGHT = -100.f;
	const float Scene::CONE_HEIGHT = -400.f;
	const float Scene::SCALE_SMALL = 0.1f;
	const float Scene::SCALE_BIG = 10.f;
#pragma endregion

	// Constructor: inicializa la escena (cámara, mallas, shaders, texturas y nodos)
	Scene::Scene(unsigned width, unsigned height)
		: angle(0),
		camera(glm::vec3(0.f, 3.f, 8.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f), // Configuración inicial de la cámara
		plane(generator.generatePlane(100, 100, 100, 100)),   // Generación del plano (heightmap)
		cone(generator.generateCone(30, 60, 10)),             // Generación del cono 3D
		ufo("../Objects/UFO.obj"),                            // Carga del modelo OVNI
		cow("../Objects/cow.obj"),                            // Carga del modelo vaca

		// Inicialización de los programas de shaders
		defaultProgram("../Code/Shaders/default_vertex_shader.glsl", "../Code/Shaders/default_fragment_shader.glsl"),
		unlitProgram("../Code/Shaders/unlit_vertex_shader.glsl", "../Code/Shaders/unlit_fragment_shader.glsl"),
		skyboxProgram("../Code/Shaders/skybox_vertex_shader.glsl", "../Code/Shaders/skybox_fragment_shader.glsl"),
		heightmapProgram("../Code/Shaders/heightmap_vertex_shader.glsl", "../Code/Shaders/default_fragment_shader.glsl"),

		// Asignación de mallas a los objetos 3D
		skybox(generator.generateCube(), &skyboxProgram),
		heightmapObj(&plane, &heightmapProgram),
		ufoObj(&ufo, &defaultProgram),
		cowObj(&cow, &defaultProgram),
		coneObj(&cone, &unlitProgram)
	{
		// Configuración inicial de OpenGL
		glEnable(GL_CULL_FACE);    // Activación del culling de caras ocultas
		glEnable(GL_DEPTH_TEST);   // Activación de la prueba de profundidad
		glClearColor(.2f, .2f, .2f, 1.f); // Color de fondo gris oscuro

		loadTextures();     // Carga y envío de texturas a GPU
		setGraph();         // Construcción del grafo de escena

		defaultProgram.use();      // Uso inicial del shader por defecto
		resize(width, height);     // Configuración inicial de proyección y viewport
	}

	// Construye la jerarquía de nodos de la escena y aplica transformaciones iniciales
	void Scene::setGraph()
	{
		rootNode = make_shared<SceneNode>();
		rootNode->setTransform({ 25.f, 0.f, -35.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f , 1.f });

		ufoCowConeNode = make_shared<SceneNode>();
		ufoCowConeNode->setTransform({ 1.2f, 0.f, 1.14f }, { 0.f, 0.f, 0.f }, { SCALE_SMALL, SCALE_SMALL, SCALE_SMALL });

		ufoNode = make_shared<SceneNode>(&ufoObj);
		ufoNode->setTransform({ 0.f, UFO_HEIGHT, 0.f }, { 90.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });

		cowNode = make_shared<SceneNode>(&cowObj);
		cowNode->setTransform({ 0.f, COW_HEIGHT, 0.f }, { 0.f, 0.f, 0.f }, { SCALE_SMALL, SCALE_SMALL, SCALE_SMALL });

		coneNode = make_shared<SceneNode>(&coneObj);
		coneNode->setTransform({ 0.f, CONE_HEIGHT, 0.f }, { 0.f, 0.f, 0.f }, { SCALE_BIG, SCALE_BIG, SCALE_BIG });

		// Añadir hijos al nodo contenedor
		ufoCowConeNode->addChild(ufoNode);
		ufoCowConeNode->addChild(cowNode);
		ufoCowConeNode->addChild(coneNode);

		// Nodo para el plano del terreno
		heightmapNode = make_shared<SceneNode>(&heightmapObj);
		heightmapNode->setTransform({ 0.f, -40.f, 0.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });

		// Añadir nodos a la raíz de la escena
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

		skybox.setTexture(skyboxTextureID);

		setTextures(); // Asignar texturas a los objetos
	}

	// Asocia los IDs de texturas cargadas a los objetos correspondientes
	void Scene::setTextures()
	{
		ufoObj.setTextureID(ufoTextureID);
		cowObj.setTextureID(cowTextureID);
		coneObj.setTextureID(coneTextureID);
		heightmapObj.setHeightmapTextureID(heightmapID);
		heightmapObj.setTextureID(heightmapTextureID);
	}

	// Actualiza la escena animando el ángulo (para efectos como rotación o flotación)
	void Scene::update()
	{
		angle += 1.0f;
	}

	// Renderiza la escena completa, incluyendo skybox, luces y objetos
	void Scene::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpieza de buffers

		glm::mat4 view_matrix = camera.get_view_matrix();   // Matriz de vista desde la cámara

		skybox.render(view_matrix, projection_matrix);      // Render del skybox
		lightSetup(view_matrix);                            // Configuración de luces

		// Animación senoidal de flotación y rotación del grupo ufo-vaca-cono
		float float_height = 2.0f;
		float y_offset = sin(angle / 100) * float_height;
		ufoCowConeNode->setTransform({ 5.f, y_offset, 5.f }, { 0.f, angle, 0.f }, { 0.1f, 0.1f, 0.1f });

		glm::mat4 identity = glm::mat4(1.f); // Matriz identidad para la raíz
		rootNode->render(identity, view_matrix, projection_matrix); // Render del grafo
	}

	// Envía información de iluminación a los shaders (dirección, color, intensidad, cámara)
	void Scene::lightSetup(glm::mat4& view_matrix)
	{
		vector<glm::vec3> lightDirsWorld = {
			glm::normalize(glm::vec3(-1.f, -1.f, -1.f)),
			glm::normalize(glm::vec3(1.f, 1.f, 1.f))
		};

		vector<glm::vec3> lightColors = {
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		};

		vector<glm::vec3> lightDirsView;
		for (const auto& dir : lightDirsWorld)
			lightDirsView.push_back(glm::mat3(view_matrix) * dir);

		vector<float> lightIntensities = { 1.0f, 1.0f };

		// Configuración de luces para el heightmap
		heightmapProgram.use();
		for (int i = 0; i < 2; ++i) {
			string index = std::to_string(i);
			heightmapProgram.setVec3("lightDirections[" + index + "]", lightDirsView[i]);
			heightmapProgram.setVec3("lightColors[" + index + "]", lightColors[i]);
			heightmapProgram.setFloat("lightIntensities[" + index + "]", lightIntensities[i]);
		}

		// Configuración de luces para el shader por defecto
		defaultProgram.use();
		for (int i = 0; i < 2; ++i) {
			string index = std::to_string(i);
			defaultProgram.setVec3("lightDirections[" + index + "]", lightDirsView[i]);
			defaultProgram.setVec3("lightColors[" + index + "]", lightColors[i]);
			defaultProgram.setFloat("lightIntensities[" + index + "]", lightIntensities[i]);
		}
		defaultProgram.setVec3("viewPos", viewPos); // Posición de la cámara
	}

	// Actualiza la proyección y el viewport al redimensionar la ventana
	void Scene::resize(unsigned width, unsigned height)
	{
		projection_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);

		defaultProgram.setMat4("projection_matrix", projection_matrix);

		glViewport(0, 0, width, height); // Ajuste del área de dibujo
	}

	// Establece una nueva cámara como activa
	void Scene::set_camera(Camera new_camera)
	{
		camera = new_camera;
	}

}
