#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include "Camera.hpp"

extern Camera* camera;
extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;
extern float dt;

extern double lastX;
extern double lastY;
extern bool firstMouse;
extern bool camMove;
extern bool lastCamMove;

// Wireframe
extern bool wireFrame;
extern bool lastF4;

GLFWwindow* setup();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void cameraUpdate(GLFWwindow* window, glm::mat4& view);