// Este código es de dominio público
// Autor: andrmatgonros@gmail.com

#pragma once

#include "../Headers/Scene.hpp"
namespace udit
{
#pragma region constants
	// Definición de constantes para alturas y escalas de objetos en la escena
	const float Scene::UFO_HEIGHT = 250.f;
	const float Scene::COW_HEIGHT = -100.f;
	const float Scene::CONE_HEIGHT = -400.f;
	const float Scene::SCALE_SMALL = 0.1f;
	const float Scene::SCALE_BIG = 10.f;
#pragma endregion

	// Constructor de la clase Scene: inicializa cámara, objetos, shaders y texturas
	Scene::Scene(unsigned width, unsigned height)
		: angle(0),
		camera(glm::vec3(0.f, 3.f, 8.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f), // Posición y orientación inicial de la cámara
		plane(generator.generatePlane(100, 100, 100, 100)),  // Genera un plano grande
		cone(generator.generateCone(30, 60, 10)),            // Genera un cono
		ufo("../Objects/UFO.obj"),                            // Carga modelo OVNI
		cow("../Objects/cow.obj"),                            // Carga modelo vaca

		// Inicializa shaders con sus respectivos códigos
		defaultProgram("../Code/Shaders/default_vertex_shader.glsl", "../Code/Shaders/default_fragment_shader.glsl"),
		unlitProgram("../Code/Shaders/unlit_vertex_shader.glsl", "../Code/Shaders/unlit_fragment_shader.glsl"),
		skyboxProgram("../Code/Shaders/skybox_vertex_shader.glsl", "../Code/Shaders/skybox_fragment_shader.glsl"),
		heightmapProgram("../Code/Shaders/heightmap_vertex_shader.glsl", "../Code/Shaders/default_fragment_shader.glsl"),

		// Inicializa objetos con sus modelos y shaders
		skybox(generator.generateCube(), &skyboxProgram),
		heightmapObj(&plane, &heightmapProgram),
		ufoObj(&ufo, &defaultProgram),
		cowObj(&cow, &defaultProgram),
		coneObj(&cone, &unlitProgram)

#pragma region Constructor
	{
		glEnable(GL_CULL_FACE);   // Activar culling para mejorar rendimiento
		glEnable(GL_DEPTH_TEST);  // Activar prueba de profundidad para render correcto
		glClearColor(.2f, .2f, .2f, 1.f);  // Color de fondo gris oscuro

		loadTextures();  // Cargar texturas en GPU

		setGraph();      // Configurar la jerarquía de nodos de la escena

		defaultProgram.use();  // Activar shader por defecto
		resize(width, height); // Configurar proyección según tamaño de ventana
	}
#pragma endregion

	// Configura la jerarquía de nodos (escena gráfica) con transformaciones y relaciones padre-hijo
	void Scene::setGraph()
	{
		rootNode = make_shared<SceneNode>(); // Nodo raíz de la escena
		rootNode->setTransform({ 25.f, 0.f, -35.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f , 1.f });

		// Nodo que agrupa OVNI, vaca y cono
		ufoCowConeNode = make_shared<SceneNode>();
		ufoCowConeNode->setTransform({ 1.2f, 0.f, 1.14f }, { 0.f, 0.f, 0.f }, { SCALE_SMALL, SCALE_SMALL, SCALE_SMALL });

		// Nodo OVNI con posición y rotación específica
		ufoNode = make_shared<SceneNode>(&ufoObj);
		ufoNode->setTransform({ 0.f, UFO_HEIGHT, 0.f }, { 90.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });

		// Nodo vaca con posición y escala pequeñas
		cowNode = make_shared<SceneNode>(&cowObj);
		cowNode->setTransform({ 0.f, COW_HEIGHT, 0.f }, { 0.f, 0.f, 0.f }, { SCALE_SMALL, SCALE_SMALL, SCALE_SMALL });

		// Nodo cono con posición y escala grande
		coneNode = make_shared<SceneNode>(&coneObj);
		coneNode->setTransform({ 0.f, CONE_HEIGHT, 0.f }, { 0.f, 0.f, 0.f }, { SCALE_BIG, SCALE_BIG, SCALE_BIG });

		// Añadir OVNI, vaca y cono al nodo contenedor
		ufoCowConeNode->addChild(ufoNode);
		ufoCowConeNode->addChild(cowNode);
		ufoCowConeNode->addChild(coneNode);

		// Nodo para el terreno con heightmap
		heightmapNode = make_shared<SceneNode>(&heightmapObj);
		heightmapNode->setTransform({ 0.f, -40.f, 0.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });

		// Añadir terreno y grupo de objetos a la raíz
		rootNode->addChild(heightmapNode);
		rootNode->addChild(ufoCowConeNode);
	}

	// Carga las texturas desde archivos y las envía a la GPU
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

		setTextures(); // Asignar texturas a los objetos cargados
	}

	// Asigna los IDs de textura a cada objeto para su uso en shaders
	void Scene::setTextures()
	{
		ufoObj.setTextureID(ufoTextureID);
		cowObj.setTextureID(cowTextureID);
		coneObj.setTextureID(coneTextureID);
		heightmapObj.setHeightmapTextureID(heightmapID);
		heightmapObj.setTextureID(heightmapTextureID);
	}

	// Actualiza el estado de la escena (por ejemplo, rotación continua de objetos)
	void Scene::update()
	{
		angle += 1.0f; // Incrementa el ángulo para animaciones
	}

	// Renderiza toda la escena
	void Scene::render() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpiar buffers de color y profundidad
		glm::mat4 view_matrix = camera.get_view_matrix();   // Obtener matriz vista de la cámara

		skybox.render(view_matrix, projection_matrix); // Renderizar skybox primero

		lightSetup(view_matrix); // Configurar luces para shaders

		// Animar el grupo de objetos OVNI-vaca-cono con movimiento flotante y rotación
		float float_height = 2.0f; // amplitud del movimiento vertical
		float y_offset = sin(angle / 100) * float_height; // desplazamiento vertical senoidal
		glm::mat4 identity = glm::mat4(1.f); // matriz identidad
		ufoCowConeNode->setTransform({ 5.f, y_offset, 5.f }, { 0.f, angle, 0.f }, { 0.1f, 0.1f, 0.1f });

		rootNode->render(identity, view_matrix, projection_matrix); // Renderizar el grafo de la escena
	}

	// Configura las propiedades y posiciones de las luces en el espacio de vista
	void Scene::lightSetup(glm::mat4& view_matrix)
	{
		// Definimos múltiples luces en coordenadas del mundo
		// Cada luz tiene una dirección y un color
		vector<glm::vec3> lightDirsWorld = {
			glm::normalize(glm::vec3(-1.f, -1.f, -1.f)),  // Luz desde dirección diagonal negativa
			glm::normalize(glm::vec3(1.f, 1.f, 1.f))      // Luz desde dirección diagonal positiva
		};

		// Colores de las luces (blancas)
		vector<glm::vec3> lightColors = {
			glm::vec3(1.0f, 1.0f, 1.0f),  // Blanco
			glm::vec3(1.0f, 1.0f, 1.0f)   // Blanco
		};

		// Transformar las direcciones de luz del sistema de coordenadas mundo al sistema de vista
		// Esto se hace para que los shaders reciban las direcciones correctamente según la cámara
		vector<glm::vec3> lightDirsView;
		for (const auto& dir : lightDirsWorld) {
			lightDirsView.push_back(glm::mat3(view_matrix) * dir);
		}

		// Intensidad de cada luz (escala de brillo)
		vector<float> lightIntensities = {
			1.0f,  // Intensidad máxima para la primera luz
			1.0f   // Intensidad máxima para la segunda luz
		};

		// Activar el programa de shaders para el heightmap
		heightmapProgram.use();

		// Enviar las propiedades de las luces al shader heightmap
		for (int i = 0; i < 2; ++i) {
			string index = std::to_string(i);
			heightmapProgram.setVec3("lightDirections[" + index + "]", lightDirsView[i]);
			heightmapProgram.setVec3("lightColors[" + index + "]", lightColors[i]);
			heightmapProgram.setFloat("lightIntensities[" + index + "]", lightIntensities[i]);
		}

		// Activar el programa de shaders por defecto para los demás objetos
		defaultProgram.use();

		// Enviar las propiedades de las luces al shader por defecto
		for (int i = 0; i < 2; ++i) {
			string index = std::to_string(i);
			defaultProgram.setVec3("lightDirections[" + index + "]", lightDirsView[i]);
			defaultProgram.setVec3("lightColors[" + index + "]", lightColors[i]);
			defaultProgram.setFloat("lightIntensities[" + index + "]", lightIntensities[i]);
		}
		// Enviar la posición de la cámara para cálculos de iluminación (por ejemplo, especular)
		defaultProgram.setVec3("viewPos", viewPos);
	}

	// Ajusta el tamaño de la ventana y actualiza la matriz de proyección
	void Scene::resize(unsigned width, unsigned height)
	{
		// Crear una matriz de proyección en perspectiva
		// 20 grados de campo de visión vertical, relación de aspecto basada en ancho y alto
		// plano cercano a 1 unidad y plano lejano a 5000 unidades
		projection_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);

		// Enviar la nueva matriz de proyección al shader por defecto
		defaultProgram.setMat4("projection_matrix", projection_matrix);

		// Ajustar el viewport OpenGL al nuevo tamaño de ventana
		glViewport(0, 0, width, height);
	}

	// Cambia la cámara actual a una nueva cámara dada
	void Scene::set_camera(Camera new_camera)
	{
		camera = new_camera;
	}
}