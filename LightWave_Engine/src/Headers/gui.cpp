#include "gui.hpp"

void GUI::objectsWindow()
{
	ImGui::Begin("Objects", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);

	std::string name = "Cubes: " + std::to_string(cubes->size());
	if (ImGui::CollapsingHeader(name.c_str())) {
		unsigned int i = 1;
		for (Object* cube : *cubes) {
			name = "Cube" + std::to_string(i);

			ImGui::SeparatorText(name.c_str());

			ImGui::BeginChild(name.c_str(), ImVec2(200, 90), ImGuiChildFlags_::ImGuiChildFlags_None, ImGuiWindowFlags_::ImGuiWindowFlags_None);

			ImGui::BeginGroup();

			ImGui::DragFloat3("Position", &cube->position.x, 0.01f);
			ImGui::DragFloat3("Rotation", &cube->rotation.x, 0.01f);
			ImGui::DragFloat3("Scale", &cube->scale.x, 0.01f);

			ImGui::DragFloat("Shininess", &cube->shininess, 1.0f, 1.0f, 1024.0f);

			ImGui::EndGroup();
			ImGui::EndChild();
			++i;
		}
	}

	name = "Plants: " + std::to_string(plants->size());
	if (ImGui::CollapsingHeader(name.c_str())) {
		unsigned int i = 1;
		for (Object* plant : *plants) {
			name = "Plant" + std::to_string(i);

			ImGui::SeparatorText(name.c_str());

			ImGui::BeginChild(name.c_str(), ImVec2(200, 90), ImGuiChildFlags_::ImGuiChildFlags_None, ImGuiWindowFlags_::ImGuiWindowFlags_None);

			ImGui::BeginGroup();

			ImGui::DragFloat3("Position", &plant->position.x, 0.01f);
			ImGui::DragFloat3("Rotation", &plant->rotation.x, 0.01f);
			ImGui::DragFloat3("Scale", &plant->scale.x, 0.01f);

			ImGui::DragFloat("Shininess", &plant->shininess, 1.0f, 1.0f, 1024.0f);

			ImGui::EndGroup();
			ImGui::EndChild();
			++i;
		}
	}

	name = "Windows: " + std::to_string(windows->size());
	if (ImGui::CollapsingHeader(name.c_str())) {
		unsigned int i = 1;
		for (Object* window : *windows) {
			name = "Window" + std::to_string(i);

			ImGui::SeparatorText(name.c_str());

			ImGui::BeginChild(name.c_str(), ImVec2(200, 90), ImGuiChildFlags_::ImGuiChildFlags_None, ImGuiWindowFlags_::ImGuiWindowFlags_None);

			ImGui::BeginGroup();

			ImGui::DragFloat3("Position", &window->position.x, 0.01f);
			ImGui::DragFloat3("Rotation", &window->rotation.x, 0.01f);
			ImGui::DragFloat3("Scale", &window->scale.x, 0.01f);

			ImGui::DragFloat("Shininess", &window->shininess, 1.0f, 1.0f, 1024.0f);

			ImGui::EndGroup();
			ImGui::EndChild();
			++i;
		}
	}

	name = "Models: " + std::to_string(models->size());
	if (ImGui::CollapsingHeader(name.c_str())) {
		unsigned int i = 1;
		for (Model* model : *models) {
			name = "Model" + std::to_string(i);

			ImGui::SeparatorText(name.c_str());

			ImGui::BeginChild(name.c_str(), ImVec2(200, 90), ImGuiChildFlags_::ImGuiChildFlags_None, ImGuiWindowFlags_::ImGuiWindowFlags_None);

			ImGui::BeginGroup();

			ImGui::DragFloat3("Position", &model->position.x, 0.01f);
			ImGui::DragFloat3("Rotation", &model->rotation.x, 0.01f);
			ImGui::DragFloat3("Scale", &model->scale.x, 0.01f);

			ImGui::DragFloat("Shininess", &model->shininess, 1.0f, 1.0f, 1024.0f);

			ImGui::EndGroup();
			ImGui::EndChild();
			++i;
		}
	}

	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::End();
}

void GUI::visualsWindow()
{
	ImGui::Begin("Visuals", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);

	ImGui::BeginTabBar("Visual Options");

	if (ImGui::TabItemButton("Lighting", ImGuiTabItemFlags_::ImGuiTabItemFlags_SetSelected)) {
		lightingMenu = true;
		cameraMenu = false;
	}
	if (ImGui::TabItemButton("Camera")) {
		lightingMenu = false;
		cameraMenu = true;
	}

	ImGui::EndTabBar();

	if (lightingMenu) lightingOptions();
	if (cameraMenu) cameraOptions();

	ImGui::SetWindowPos(ImVec2(*width - (ImGui::GetWindowSize().x), 0));
	ImGui::End();
}

void GUI::lightingOptions()
{
	ImGui::SeparatorText("Directional");
	ImGui::BeginChild("Directional", ImVec2(250, 50));
	ImGui::BeginGroup();

	ImGui::DragFloat3("Direction", lightdirection, 0.01f);
	ImGui::ColorEdit3("Color", dirColor, 0.01f);

	ImGui::EndGroup();
	ImGui::EndChild();

	std::string name = "PointLights: " + std::to_string(lightingCubes->size());
	if (ImGui::CollapsingHeader(name.c_str())) {
		unsigned int i = 1;
		for (LightCube* cube : *lightingCubes) {
			name = "LighingCube" + std::to_string(i);

			ImGui::SeparatorText(name.c_str());

			ImGui::BeginChild(name.c_str(), ImVec2(250, 90));

			ImGui::BeginGroup();

			ImGui::DragFloat3("Position", &cube->position.x, 0.01f);
			ImGui::DragFloat3("Scale", &cube->scale.x, 0.01f);
			ImGui::ColorEdit3("Color", &cube->color.x, 0.01f);

			ImGui::EndGroup();
			ImGui::EndChild();
			++i;
		}
	}
}

void GUI::cameraOptions()
{
	ImGui::SeparatorText("Camera Options");

	ImGui::BeginGroup();

	ImGui::DragFloat3("Position", cameraPos, 0.01f);
	ImGui::DragFloat3("Rotation", cameraRotation, 0.01f);
	if (cameraRotation[1] < -1.0f) cameraRotation[1] = -1.0f;
	if (cameraRotation[1] > 1.0f) cameraRotation[1] = 1.0f;
	ImGui::DragFloat("Speed", speed, 0.01f, 0.5f, 50.0f);
	ImGui::SliderFloat("FOV", fov, 1.0f, 180.0f);

	if (ImGui::Button("Reset")) {
		*fov = 45.0f;
		*speed = 2.5f;
	}

	ImGui::EndGroup();
}

void GUI::frameRate()
{
	ImGui::Begin("FrameRate", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::SeparatorText("DemoWindow");

	ImGui::Checkbox("Open the demo window.", &show_demo_window);

	ImGui::SeparatorText("FrameRate");

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

	ImGui::End();
}

void GUI::cameraLocked()
{
	ImGui::Begin("Camera Locked", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);
	ImGui::Text("The camera is locked. Press 'escape' to unlock it.");
	ImGui::SetWindowPos(ImVec2(*width / 2.0f - (ImGui::GetWindowSize().x / 2.0f), *height - 100.0f));
	ImGui::End();
}

GUI::GUI(unsigned int* width, unsigned int* height, GLFWwindow* window) : width(width), height(height)
{
	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void GUI::update()
{
	// UI
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	objectsWindow();
	visualsWindow();
	frameRate();

	if (show_demo_window) {
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) moveCam = !moveCam;
	if (!moveCam) {
		cameraLocked();
	}
}

void GUI::draw()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
