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
#include "LightSystem.hpp"

class GUI {
	ImGuiIO& io = ImGui::GetIO();

private:
	bool show_demo_window = false;
	bool lightingMenu = true;
	bool cameraMenu = false;
	bool addPointLight = false;
	bool addDirectionalLight = false;
	bool addSpotLight = false;

private:
	void objectsWindow();
	void visualsWindow();
	void lightingOptions();
	void cameraOptions();
	void frameRate();
	void cameraLocked();

public:
	Objects& cubes;
	LightSystem& lightingSystem;
	Transparents& transparents;
	Models& models;

public:
	GUI(GLFWwindow* window,
		Objects& cubes,
		LightSystem& lightingSystem,
		Transparents& transparents,
		Models& models);

public:
	void update();
	void draw();
};

#endif // !GUI_HPP