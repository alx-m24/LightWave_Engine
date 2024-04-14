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
#include "Headers/Camera.hpp"
#include "Headers/Textures/Textures.hpp"
#include "Headers/Model.hpp"
#include "Headers/Object.hpp"
#include "Headers/gui.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void cameraUpdate(GLFWwindow* window, GUI& gui, glm::mat4& view);

// settings
unsigned int SCR_WIDTH = 1000;
unsigned int SCR_HEIGHT = 600;

// Wireframe
bool wireFrame = false;
bool lastF4;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool* camMove;

// Time
float dt = 0.0f;
float lastFrame = 0.0f;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LightWave_Engine", NULL, NULL);
	if (window == nullptr) {
		std::cerr << "Failed to create window" << std::endl;
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	stbi_set_flip_vertically_on_load(true);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Triangle
	Shader lightCube("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Shaders\\src\\lightCube.vert", "C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Shaders\\src\\lightCube.frag");
	Shader lightingShader("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Shaders\\src\\LightEnvironment.vert", "C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Shaders\\src\\LightEnvironment.frag");
	Shader transParentShader("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Shaders\\src\\plant.vert", "C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Shaders\\src\\plant.frag");

	float vertices[] = {
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

	glm::vec3 cubePositions[] = {
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

	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

	float transparentVertices[] = {
		// positions         // texture Coords
		0.0f,  0.5f,  0.0f,  1.0f,  1.0f,
		0.0f, -0.5f,  0.0f,  1.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  0.0f,  0.0f,

		0.0f,  0.5f,  0.0f,  1.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  0.0f,  0.0f,
		1.0f,  0.5f,  0.0f,  0.0f,  1.0f
	};

	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Textures
	unsigned int diffuseMap = loadTexture("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Textures\\src\\container2.png");
	unsigned int specularMap = loadTexture("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Textures\\src\\container2_specular.png");
	unsigned int grass = loadTexture("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Textures\\src\\grass.png");
	unsigned int windowTex = loadTexture("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Textures\\src\\blending_transparent_window.png");

	glm::vec3 pos[]{
		glm::vec3(-1.5f,  0.0f, -0.48f), glm::vec3(1.5f,  0.0f,  0.51f), glm::vec3(0.0f,  0.0f,  0.7f), glm::vec3(-0.3f,  0.0f, -2.3f), glm::vec3(0.5f,  0.0f, -0.6f)
	};

	std::vector<Object*> transparent;

	std::vector<Object*> plants;
	for (int i = 0; i < 4; ++i) {
		Object* temp = new Object(planeVAO, grass, NULL);
		temp->position = pos[i];
		plants.push_back(temp);
		transparent.push_back(temp);
	}

	std::vector<Object*> windows;
	for (int i = 0; i < 4; ++i) {
		Object* temp = new Object(planeVAO, windowTex, NULL);
		temp->position = pos[i] + glm::vec3(rand() % 5, rand() % 5, rand() % 5);
		windows.push_back(temp);
		transparent.push_back(temp);
	}

	std::vector<Object*> cubes;
	for (int i = 0; i < std::size(cubePositions); ++i) {
		Object* cube = new Object(cubeVAO, diffuseMap, specularMap);
		cube->setPosition(cubePositions[i]);
		cube->setRotation(glm::vec3(1.0f, 0.3f, 0.5f) * float(i * 20));
		cubes.push_back(cube);
	}

	std::vector<LightCube*> lightCubes;
	for (int i = 0; i < std::size(pointLightPositions); ++i) {
		LightCube* cube = new LightCube(lightVAO);
		cube->color = lightColor[i];
		cube->setPosition(pointLightPositions[i]);
		cube->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
		lightCubes.push_back(cube);
	}

	std::vector<Model*> models;
	Model* backbag = new Model("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Models\\BackBag\\backpack.obj");
	backbag->position = glm::vec3(-1, 0.5, -3.5);
	backbag->scale = glm::vec3(0.2f, 0.2f, 0.2f);
	backbag->shininess = 32.0f;
	models.push_back(backbag);

	Model* table = new Model("C:\\Users\\alexa\\OneDrive\\Coding\\C++\\LightWave_Engine\\LightWave_Engine\\src\\Headers\\Models\\Table\\source\\SIMPLE ROUND TABLE.obj");
	table->shininess = 16.0f;
	models.push_back(table);

	glm::mat4 view = camera.GetViewMatrix();
	glm::vec3 lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 colorDir = glm::vec3(1.0f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	GUI gui(&SCR_WIDTH, &SCR_HEIGHT, window);

	gui.cubes = &cubes;
	gui.lightingCubes = &lightCubes;
	gui.plants = &plants;
	gui.models = &models;
	gui.windows = &windows;
	gui.lightdirection = &lightDir.x;
	gui.dirColor = &colorDir.x;

	gui.cameraPos = &camera.Position.x;
	gui.cameraRotation = &camera.Front.x;
	gui.speed = &camera.MovementSpeed;
	gui.fov = &camera.Zoom;

	camMove = &gui.moveCam;

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
			std::string name = "pointLights[" + std::to_string(i) + "]";

			lightingShader.setVec3(name + ".position", lightCubes[i]->getPosition());
			lightingShader.setVec3(name + ".ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.setVec3(name + ".diffuse", 0.8f, 0.8f, 0.8f);
			lightingShader.setVec3(name + ".specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat(name + ".constant", 1.0f);
			lightingShader.setFloat(name + ".linear", 0.09f);
			lightingShader.setFloat(name + ".quadratic", 0.032f);
			lightingShader.setVec3(name + ".color", lightCubes[i]->color);
		}

		// Directional
		lightingShader.setVec3("dirLight.direction", lightDir);
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("dirLight.color", colorDir);
		// spotLight
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


		cameraUpdate(window, gui, view);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		glBindVertexArray(cubeVAO);
		for (Object* cube : cubes) {
			cube->update(&lightingShader);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glFrontFace(GL_CCW);
		for (Model* model : models) {
			model->draw(&lightingShader);
		}
		glFrontFace(GL_CW);

		lightCube.use();
		lightCube.setMat4("view", view);
		lightCube.setMat4("projection", projection);
		glBindVertexArray(lightVAO);
		for (LightCube* cube : lightCubes) {
			lightCube.setVec3("color", cube->color);
			cube->update(&lightCube);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		std::map<float, Object*> sorted;
		for (unsigned int i = 0; i < transparent.size(); i++)
		{
			float distance = glm::length(camera.Position - transparent[i]->position);
			sorted[distance] = transparent[i];
		}

		transParentShader.use();
		transParentShader.setMat4("view", view);
		transParentShader.setMat4("projection", projection);
		glDisable(GL_CULL_FACE);
		glBindVertexArray(planeVAO);
		for (std::map<float, Object*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
			it->second->update(&transParentShader);
			glDrawArrays(GL_TRIANGLES, 0, 6);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	if (!*camMove) return;

	camera.rotate(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (!*camMove) return;
	camera.zoom(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
	bool F4 = glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS;
	if (F4 && !lastF4) {
		wireFrame = !wireFrame;

		if (wireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	lastF4 = F4;

	if (!*camMove) return;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.move(Camera::FORWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.move(Camera::BACKWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.move(Camera::LEFT, dt);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.move(Camera::RIGHT, dt);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.move(Camera::FORWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.move(Camera::BACKWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.move(Camera::LEFT, dt);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.move(Camera::RIGHT, dt);
}

void cameraUpdate(GLFWwindow* window, GUI& gui, glm::mat4& view) {
	if (gui.moveCam && !gui.lastmoveCam) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		float pitch = std::asin(camera.Front.y);
		float yaw = std::atan2(camera.Front.z, camera.Front.x);

		camera.Pitch = glm::degrees(pitch);
		camera.Yaw = glm::degrees(yaw);
	}
	if (!gui.moveCam && gui.lastmoveCam) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPos(window, SCR_WIDTH / 2.0, SCR_HEIGHT / 2);
	}

	view = camera.GetViewMatrix();

	gui.lastmoveCam = gui.moveCam;
}