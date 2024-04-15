#pragma once
#ifndef GUI_HPP
#define GUI_HPP

#include <imgui.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
#include "Object.hpp"
#include "Model.hpp"
#include "Useful.hpp"

class GUI {
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
	Objects& cubes;
	std::vector<Model>& models;
	LighCubes& lightingCubes;
	std::vector<Transparent*>& transparent;
	float* lightdirection = nullptr;
	float* dirColor = nullptr;

public:
	GUI(GLFWwindow* window, 
		Objects& cubes, 
		std::vector<Transparent*>& transparent,
		std::vector<Model>& models,
		LighCubes& lightingCubes);

public:
	void update();
	void draw();
};

#endif // !GUI_HPP