#include "gui.hpp"

void GUI::objectsWindow()
{
	ImGui::Begin("Objects", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);

	std::string name = "Cubes: " + std::to_string(cubes.size());
	if (ImGui::CollapsingHeader(name.c_str())) {
		if (ImGui::Button("Add Cube")) {
			cubes.emplace_back(Object(glm::vec3(0.0f)));
		}

		unsigned int i = 1;
		for (Object& cube : cubes) {
			name = "Cube" + std::to_string(i);

			ImGui::SeparatorText(name.c_str());

			ImGui::BeginChild(name.c_str(), ImVec2(200, 90), ImGuiChildFlags_::ImGuiChildFlags_None, ImGuiWindowFlags_::ImGuiWindowFlags_None);

			ImGui::BeginGroup();

			ImGui::DragFloat3("Position", &cube.position.x, 0.01f);
			ImGui::DragFloat3("Rotation", &cube.rotation.x, 0.01f);
			ImGui::DragFloat3("Scale", &cube.scale.x, 0.01f);

			ImGui::DragFloat("Shininess", &cube.shininess, 1.0f, 1.0f, 1024.0f);

			ImGui::EndGroup();
			ImGui::EndChild();
			++i;
		}
	}

	name = "Transparent Objects: " + std::to_string(transparents.size());
	if (ImGui::CollapsingHeader(name.c_str())) {
		unsigned int i = 1;
		for (Transparent& transparentObj : transparents) {
			name = "Transparent" + std::to_string(i);

			ImGui::SeparatorText(name.c_str());

			ImGui::BeginChild(name.c_str(), ImVec2(200, 90), ImGuiChildFlags_::ImGuiChildFlags_None, ImGuiWindowFlags_::ImGuiWindowFlags_None);

			ImGui::BeginGroup();

			ImGui::DragFloat3("Position", &transparentObj.position.x, 0.01f);
			ImGui::DragFloat3("Rotation", &transparentObj.rotation.x, 0.01f);
			ImGui::DragFloat3("Scale", &transparentObj.scale.x, 0.01f);

			ImGui::DragFloat("Shininess", &transparentObj.shininess, 1.0f, 1.0f, 1024.0f);

			ImGui::EndGroup();
			ImGui::EndChild();
			++i;
		}
	}

	name = "Models: " + std::to_string(models.size());
	if (ImGui::CollapsingHeader(name.c_str())) {
		unsigned int i = 1;
		for (Model& model : models) {
			name = "Model" + std::to_string(i);

			ImGui::SeparatorText(name.c_str());

			ImGui::BeginChild(name.c_str(), ImVec2(200, 90), ImGuiChildFlags_::ImGuiChildFlags_None, ImGuiWindowFlags_::ImGuiWindowFlags_None);

			ImGui::BeginGroup();

			ImGui::DragFloat3("Position", &model.position.x, 0.01f);
			ImGui::DragFloat3("Rotation", &model.rotation.x, 0.01f);
			ImGui::DragFloat3("Scale", &model.scale.x, 0.01f);

			ImGui::DragFloat("Shininess", &model.shininess, 1.0f, 1.0f, 1024.0f);

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

	ImGui::SetWindowPos(ImVec2(SCR_WIDTH - (ImGui::GetWindowSize().x), 0));
	ImGui::End();
}

void GUI::lightingOptions()
{
	std::string name = "DirectionalLights: " + std::to_string(lightingSystem.directionalLights.size());
	if (ImGui::CollapsingHeader(name.c_str())) {
		if (ImGui::Button("Add DirectionalLight")) addDirectionalLight = true;

		if (addDirectionalLight) {
			static glm::vec3 color(1.0f);
			ImGui::Begin("Add DirectionalLight");

			ImGui::ColorPicker3("Color", &color.x);
			if (ImGui::Button("Done")) {
				lightingSystem.addDirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f), color);
				addDirectionalLight = false;
			}

			ImGui::End();
		}

		unsigned int i = 1;
		std::vector<DirectionalLight>& lights = lightingSystem.directionalLights;
		for (DirectionalLight& light : lights) {
			name = "Directional" + std::to_string(i);

			ImGui::SeparatorText(name.c_str());

			ImGui::BeginChild(name.c_str(), ImVec2(250, 130));

			ImGui::BeginGroup();

			ImGui::DragFloat3("Direction", &light.direction.x, 0.01f);

			ImGui::ColorEdit3("Ambient", &light.ambient.x, 0.01f);
			ImGui::ColorEdit3("Diffuse", &light.diffuse.x, 0.01f);
			ImGui::ColorEdit3("Specular", &light.specular.x, 0.01f);

			ImGui::ColorEdit3("Color", &light.color.x, 0.01f);

			ImGui::EndGroup();
			ImGui::EndChild();
			++i;
		}
	}

	name = "PointLights: " + std::to_string(lightingSystem.pointlights.size());
	if (ImGui::CollapsingHeader(name.c_str())) {
		if (ImGui::Button("Add PointLight")) addPointLight = true;

		if (addPointLight) {
			static glm::vec3 color(1.0f);
			ImGui::Begin("Add PointLight");

			ImGui::ColorPicker3("Color", &color.x);
			if (ImGui::Button("Done")) {
				lightingSystem.addPointLight(glm::vec3(0.0f), color);
				addPointLight = false;
			}

			ImGui::End();
		}

		unsigned int i = 1;
		std::vector<PointLight>& lights = lightingSystem.pointlights;
		for (PointLight& light : lights) {
			name = "PointLight" + std::to_string(i);

			ImGui::SeparatorText(name.c_str());

			ImGui::BeginChild(name.c_str(), ImVec2(250, 140));

			ImGui::BeginGroup();

			ImGui::DragFloat3("Position", &light.position.x, 0.01f);
			ImGui::DragFloat3("Scale", &light.scale.x, 0.01f);

			ImGui::ColorEdit3("Ambient", &light.ambient.x, 0.01f);
			ImGui::ColorEdit3("Diffuse", &light.diffuse.x, 0.01f);
			ImGui::ColorEdit3("Specular", &light.specular.x, 0.01f);

			ImGui::ColorEdit3("Color", &light.color.x, 0.01f);

			ImGui::EndGroup();
			ImGui::EndChild();
			++i;
		}
	}

	name = "SpotLights: " + std::to_string(lightingSystem.spotLights.size());
	if (ImGui::CollapsingHeader(name.c_str())) {
		if (ImGui::Button("Add SpotLight")) addSpotLight = true;

		if (addSpotLight) {
			static glm::vec3 color(1.0f);
			static bool flashLight = false;

			ImGui::Begin("Add SpotLight");

			ImGui::ColorPicker3("Color", &color.x);
			ImGui::Checkbox("Flash Light", &flashLight);
			if (ImGui::Button("Done")) {
				lightingSystem.addSpotLight(flashLight, glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), color);
				addSpotLight = false;
			}

			ImGui::End();
		}

		unsigned int i = 1;
		std::vector<SpotLight>& lights = lightingSystem.spotLights;
		for (SpotLight& light : lights) {
			name = "SpotLight" + std::to_string(i);

			ImGui::SeparatorText(name.c_str());

			ImGui::BeginChild(name.c_str(), ImVec2(250, 160));

			ImGui::BeginGroup();

			ImGui::DragFloat3("Position", &light.pos.x, 0.01f);
			ImGui::DragFloat3("Direction", &light.dir.x, 0.01f);

			ImGui::ColorEdit3("Ambient", &light.ambient.x, 0.01f);
			ImGui::ColorEdit3("Diffuse", &light.diffuse.x, 0.01f);
			ImGui::ColorEdit3("Specular", &light.specular.x, 0.01f);

			ImGui::ColorEdit3("Color", &light.color.x, 0.01f);

			ImGui::Checkbox("FlashLight", &light.flashLight);

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

	ImGui::DragFloat3("Position", &camera.Position.x, 0.01f);

	ImGui::DragFloat3("Rotation", &camera.Front.x, 0.01f);
	if (camera.Front.y < -1.0f) camera.Front.y = -1.0f;
	if (camera.Front.y > 1.0f) camera.Front.y = 1.0f;

	ImGui::DragFloat("Speed", &camera.MovementSpeed, 0.01f, 0.5f, 50.0f);
	ImGui::SliderFloat("Sensitivity", &camera.MouseSensitivity, 0.0001f, 1.0f);
	ImGui::SliderFloat("FOV", &camera.Zoom, 1.0f, 180.0f);

	if (ImGui::Button("Reset")) {
		camera.Zoom = 45.0f;
		camera.MouseSensitivity = 0.2f;
		camera.MovementSpeed = 2.5f;
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
	ImGui::SetWindowPos(ImVec2(SCR_WIDTH / 2.0f - (ImGui::GetWindowSize().x / 2.0f), SCR_HEIGHT - 100.0f));
	ImGui::End();
}

GUI::GUI(GLFWwindow* window, Objects& cubes, LightSystem& lightingSystem, Transparents& transparents, Models& models) : 
	cubes(cubes), lightingSystem(lightingSystem), transparents(transparents), models(models)
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

	if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) camMove = !camMove;
	if (!camMove) {
		cameraLocked();
	}
}

void GUI::draw()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}