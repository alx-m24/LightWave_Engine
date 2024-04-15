#include "Useful.hpp"

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

	if (!camMove) return;

	camera.rotate(xoffset, yoffset);
}

GLFWwindow* setup()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LightWave_Engine", NULL, NULL);
	if (window == nullptr) {
		std::cerr << "Failed to create window" << std::endl;
		return nullptr;
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
		return nullptr;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return window;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (!camMove) return;
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

	if (!camMove) return;
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

void cameraUpdate(GLFWwindow* window, glm::mat4& view) {
	if (camMove && !lastCamMove) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		float pitch = std::asin(camera.Front.y);
		float yaw = std::atan2(camera.Front.z, camera.Front.x);

		camera.Pitch = glm::degrees(pitch);
		camera.Yaw = glm::degrees(yaw);
	}
	if (!camMove && lastCamMove) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPos(window, SCR_WIDTH / 2.0, SCR_HEIGHT / 2);
	}

	view = camera.GetViewMatrix();

	lastCamMove = camMove;
}