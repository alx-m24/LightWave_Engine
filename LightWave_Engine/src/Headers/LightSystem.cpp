#include "LightSystem.hpp"

void LightSystem::updatePointLights()
{
	for (int i = 0; i < pointlights.size(); ++i) {
		PointLight& pointLight = pointlights[i];

		std::string name = "pointLights[" + std::to_string(i) + "].";

		lightingShader.setVec3(name + "position", pointLight.position);

		lightingShader.setVec3(name + "ambient", pointLight.ambient * pointLight.color);
		lightingShader.setVec3(name + "diffuse", pointLight.diffuse * pointLight.color);
		lightingShader.setVec3(name + "specular", pointLight.specular * pointLight.color);

		lightingShader.setFloat(name + "constant", pointLight.constant);
		lightingShader.setFloat(name + "linear", pointLight.linear);
		lightingShader.setFloat(name + "quadratic", pointLight.quadratic);
	}
	lightingShader.setInt("pointLightNum", pointlights.size());
}

void LightSystem::updateDirLights()
{
	for (int i = 0; i < directionalLights.size(); ++i) {
		DirectionalLight& dirLight = directionalLights[i];

		std::string name = "dirLights[" + std::to_string(i) + "].";

		lightingShader.setVec3(name + "direction", dirLight.direction);

		lightingShader.setVec3(name + "ambient", dirLight.ambient * dirLight.color);
		lightingShader.setVec3(name + "diffuse", dirLight.diffuse * dirLight.color);
		lightingShader.setVec3(name + "specular", dirLight.specular * dirLight.color);
	}
	lightingShader.setInt("dirLightNum", directionalLights.size());
}

void LightSystem::updateSpotLights()
{
	for (int i = 0; i < spotLights.size(); ++i) {
		SpotLight& spotLight = spotLights[i];

		spotLight.update();

		std::string name = "spotLights[" + std::to_string(i) + "].";

		lightingShader.setVec3(name + "position", spotLight.pos);
		lightingShader.setVec3(name + "direction", spotLight.dir);

		lightingShader.setVec3(name + "ambient", spotLight.ambient * spotLight.color);
		lightingShader.setVec3(name + "diffuse", spotLight.diffuse * spotLight.color);
		lightingShader.setVec3(name + "specular", spotLight.specular * spotLight.color);

		lightingShader.setFloat(name + "constant", spotLight.constant);
		lightingShader.setFloat(name + "linear", spotLight.linear);
		lightingShader.setFloat(name + "quadratic", spotLight.quadratic);

		lightingShader.setFloat(name + "cutOff", spotLight.cutOff);
		lightingShader.setFloat(name + "outerCutOff", spotLight.outerCutOff);
	}
	lightingShader.setInt("spotLightNum", spotLights.size());
}

void LightSystem::update()
{
	lightingShader.use();
	this->updatePointLights();
	this->updateDirLights();
	this->updateSpotLights();
}

void LightSystem::draw(Shader& shader)
{
	glBindVertexArray(cubeVAO);
	for (LightCube& cube : pointlights) {
		cube.update(shader);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}


void LightSystem::addPointLight(glm::vec3 position, glm::vec3 color)
{
	pointlights.emplace_back(PointLight());

	PointLight& pointlight = pointlights.back();

	pointlight.position = position;
	pointlight.color = color;
	pointlight.setScale(glm::vec3(0.2f, 0.2f, 0.2f));
}

void LightSystem::addDirectionalLight(glm::vec3 dir, glm::vec3 color)
{
	directionalLights.emplace_back(DirectionalLight());

	DirectionalLight& dirLight = directionalLights.back();

	dirLight.direction = dir;
	dirLight.color = color;
}

void LightSystem::addSpotLight(bool flashLight, glm::vec3 position, glm::vec3 dir, glm::vec3 color, float cutOff, float outerCutOff)
{
	spotLights.emplace_back(SpotLight());

	SpotLight& spotLight = spotLights.back();

	spotLight.cutOff = cutOff;
	spotLight.outerCutOff = outerCutOff;

	spotLight.color = color;

	spotLight.flashLight = flashLight;
	if (flashLight) return;

	spotLight.pos = position;
	spotLight.dir = dir;
}

void SpotLight::update()
{
	if (!this->flashLight) return;

	this->pos = camera.Position;
	this->dir = camera.Front;
}
