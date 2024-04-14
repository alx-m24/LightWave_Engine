#pragma once
#ifndef GUI_HPP
#define GUI_HPP

#include <imgui.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
#include "Object.hpp"
#include "Model.hpp"

class GUI {
private:
	unsigned int* width = nullptr;
	unsigned int* height = nullptr;
	ImGuiIO& io = ImGui::GetIO();

private:
	bool show_demo_window = false;
	bool lightingMenu = true;
	bool cameraMenu = false;

private:
	void objectsWindow();
	void visualsWindow();
	void lightingOptions();
	void cameraOptions();
	void frameRate();
	void cameraLocked();

public:
	bool moveCam = true;
	bool lastmoveCam = true;

public:
	std::vector<Object*>* cubes = nullptr;
	std::vector<Object*>* plants = nullptr;
	std::vector<Object*>* windows = nullptr;
	std::vector<Model*>* models = nullptr;
	LighCubes* lightingCubes = nullptr;
	float* lightdirection = nullptr;
	float* dirColor = nullptr;
	float* cameraPos = nullptr;
	float* cameraRotation = nullptr;
	float* speed = nullptr;
	float* fov = nullptr;

public:
	GUI(unsigned int* width, unsigned int* height, GLFWwindow* window);

public:
	void update();
	void draw();
};

#endif // !GUI_HPP
