#pragma once
#include <glm/glm.hpp>
#include "Shaders/Shader.hpp"
#include "Object.hpp"
#include "Useful.hpp"

struct DirectionalLight {
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

	glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 diffuse = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);

	glm::vec3 color = glm::vec3(1.0f);
};

struct SpotLight {
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 color = glm::vec3(1.0f);
	glm::vec3 dir = glm::vec3(0.0f);

	float cutOff = glm::cos(glm::radians(12.5f));
	float outerCutOff = glm::cos(glm::radians(15.0f));

	glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	bool flashLight = false;

	void update();
};

class PointLight : public LightCube {
public:
	glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};

class LightSystem {
private:
	Shader& lightingShader;

private:
	unsigned int cubeVAO;

private:
	void updatePointLights();
	void updateDirLights();
	void updateSpotLights();

public:
	std::vector<PointLight> pointlights;
	std::vector<DirectionalLight> directionalLights;
	std::vector<SpotLight> spotLights;

public:
	LightSystem(Shader& shader, unsigned int cubeVAO) : lightingShader(shader), cubeVAO(cubeVAO) {};

public:
	void update();
	void draw(Shader& shader);

	void addPointLight(glm::vec3 position = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f));
	void addDirectionalLight(glm::vec3 dir = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f));
	void addSpotLight(bool flashLight = false, glm::vec3 position = glm::vec3(0.0f), glm::vec3 dir = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f), float cutOff = glm::cos(glm::radians(12.5f)), float outerCutOff = glm::cos(glm::radians(15.0f)));
};