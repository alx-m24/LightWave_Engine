// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Others
#include <iostream>
#include <map>
#include <vector>
// My headers
#include "Headers/Shaders/Shader.hpp"
#include "Headers/Camera/Camera.hpp"
#include "Headers/Textures/Textures.hpp"
#include "Headers/Model.hpp"
#include "Headers/Object.hpp"
#include "Headers/gui.hpp"
#include "Headers/Useful.hpp"

// settings
unsigned int SCR_WIDTH = 1000;
unsigned int SCR_HEIGHT = 600;

// camera
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Hide camera
bool camMove = true;
bool lastCamMove = false;

// WireFrame
bool wireFrame = false;
bool lastF4;

// Time
float dt = 0.0f;
float lastFrame = 0.0f;

int main() {
	// Creating window and setting up opengl with glad
	GLFWwindow* window = setup();

	// Shaders
	Shader lightCube("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Shaders\\lightCube.vert", "C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Shaders\\lightCube.frag");
	Shader lightingShader("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Shaders\\LightEnvironment.vert", "C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Shaders\\LightEnvironment.frag");
	Shader transparentShader("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Shaders\\transparent.vert", "C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Shaders\\transparent.frag");

	// Textures
	unsigned int containerDiffuse = loadTexture("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Textures\\container2.png");
	unsigned int containerSpecular = loadTexture("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Textures\\container2_specular.png");
	unsigned int grass = loadTexture("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Textures\\grass.png");
	unsigned int windowTex = loadTexture("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Textures\\blending_transparent_window.png");

	float CubeVertices[] = {
		// Position				// Normal				// TexCoords
			// Back face
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f, // top-right
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f, // top-right
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f, // bottom-left
		// Front face
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	  1.0f, 1.0f, // top-right
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	  1.0f, 1.0f, // top-right
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,    0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	  0.0f, 1.0f, // top-left
		// Left face
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-right
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-left
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-right
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // bottom-right
		// Right face
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-left
		 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-right
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-right
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-left
		 // Bottom face
		 -0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		  0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // bottom-left
		  0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // top-left
		  0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // bottom-left
		 -0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // top-right
		 -0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,  0.0f, 0.0f, // bottom-right
		 // Top face
		 -0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // top-left
		  0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // top-right
		  0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // bottom-right
		  0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // bottom-right
		 -0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,  0.0f, 0.0f, // bottom-left
		 -0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,  0.0f, 1.0f  // top-left
	};
	float planeVertices[] = {
		// positions         // texture Coords
		0.0f,  0.5f,  0.0f,  1.0f,  1.0f,
		0.0f, -0.5f,  0.0f,  1.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  0.0f,  0.0f,

		0.0f,  0.5f,  0.0f,  1.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  0.0f,  0.0f,
		1.0f,  0.5f,  0.0f,  0.0f,  1.0f
	};

	glm::vec3 containerPositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	glm::vec3 lightColor[] = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};
	glm::vec3 grassPos[]{
		glm::vec3(-1.5f,  0.0f, -0.48f), glm::vec3(1.5f,  0.0f,  0.51f), glm::vec3(0.0f,  0.0f,  0.7f), glm::vec3(-0.3f,  0.0f, -2.3f), glm::vec3(0.5f,  0.0f, -0.6f)
	};

	unsigned int cubeVAO, cubeVBO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int planeVAO, planeVBO;

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	std::vector<Transparent*> transparent;

	for (int i = 0; i < 4; ++i) {
		Transparent* temp = new Transparent(planeVAO, 6, grass, NULL);

		temp->position = grassPos[i];
		transparent.push_back(temp);
	}

	for (int i = 0; i < 4; ++i) {
		Transparent* temp = new Transparent(planeVAO, 6, windowTex, windowTex);

		temp->position = grassPos[i] + glm::vec3(rand() % 5, rand() % 5, rand() % 5);
		transparent.push_back(temp);
	}

	Objects containers(cubeVAO, 36, containerDiffuse, containerSpecular);
	for (int i = 0; i < std::size(containerPositions); ++i) {
		containers.emplace_back(Object(containerPositions[i], glm::vec3(1.0f, 0.3f, 0.5f)* float(i * 20)));
	}

	LighCubes lightCubes(lightVAO);
	for (int i = 0; i < std::size(pointLightPositions); ++i) {
		lightCubes.emplace_back(LightCube(lightColor[i]));

		Object& cube = lightCubes.back();

		cube.setPosition(pointLightPositions[i]);
		cube.setScale(glm::vec3(0.2f, 0.2f, 0.2f));
	}

	std::vector<Model> models;

	{
		models.emplace_back("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Models\\BackBag\\backpack.obj");
		Model& backbag = models.back();

		backbag.position = glm::vec3(-1, 0.5, -3.5);
		backbag.scale = glm::vec3(0.2f, 0.2f, 0.2f);
		backbag.shininess = 32.0f;
	}

	{
		models.emplace_back("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\res\\Models\\Table\\source\\SIMPLE ROUND TABLE.obj");
		Model& table = models.back();

		table.position = glm::vec3(0.0f, 0.0f, -1.5f);
		table.shininess = 16.0f;

		models.push_back(table);
	}

	glm::mat4 view = camera.GetViewMatrix();
	glm::vec3 lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 colorDir = glm::vec3(1.0f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	GUI gui(window, containers, transparent, models, lightCubes);

	gui.lightdirection = &lightDir.x;
	gui.dirColor = &colorDir.x;

	while (!glfwWindowShouldClose(window)) {
		float time = glfwGetTime();
		dt = time - lastFrame;
		lastFrame = time;

		processInput(window);

		// UI
		gui.update();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) lightingShader.setBool("spot", true);
		else lightingShader.setBool("spot", false);

		// point lights
		for (int i = 0; i < lightCubes.size(); ++i) {
			const Object& cube = lightCubes[i];
			std::string name = "pointLights[" + std::to_string(i) + "]";

			lightingShader.setVec3(name + ".position", cube.position);
			lightingShader.setVec3(name + ".ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.setVec3(name + ".diffuse", 0.8f, 0.8f, 0.8f);
			lightingShader.setVec3(name + ".specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat(name + ".constant", 1.0f);
			lightingShader.setFloat(name + ".linear", 0.09f);
			lightingShader.setFloat(name + ".quadratic", 0.032f);
			lightingShader.setVec3(name + ".color", cube.color);
		}

		// Directional
		{
			lightingShader.setVec3("dirLight.direction", lightDir);
			lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
			lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
			lightingShader.setVec3("dirLight.color", colorDir);
		}

		// spotLight
		{
			lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("spotLight.constant", 1.0f);
			lightingShader.setFloat("spotLight.linear", 0.09f);
			lightingShader.setFloat("spotLight.quadratic", 0.032f);
			lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
			lightingShader.setVec3("spotLight.color", lightColor[0]);
			lightingShader.setVec3("spotLight.position", camera.Position);
			lightingShader.setVec3("spotLight.direction", camera.Front);
		}

		cameraUpdate(window, view);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		containers.draw(lightingShader);

		glFrontFace(GL_CCW);
		for (Model& model : models) {
			model.draw(&lightingShader);
		}
		glFrontFace(GL_CW);

		lightCube.use();

		lightCube.setMat4("view", view);
		lightCube.setMat4("projection", projection);

		lightCubes.draw(lightCube);

		std::map<float, Transparent*> sorted;
		for (Transparent* temp : transparent)
		{
			float distance = glm::length(camera.Position - temp->position);
			sorted[distance] = temp;
		}

		transparentShader.use();

		transparentShader.setMat4("view", view);
		transparentShader.setMat4("projection", projection);

		glDisable(GL_CULL_FACE);
		for (std::map<float, Transparent*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
			it->second->draw(transparentShader);
		}
		glEnable(GL_CULL_FACE);

		gui.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return EXIT_SUCCESS;
}