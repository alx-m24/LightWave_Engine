#pragma once
#ifndef CUBE_HPP
#define CUBDE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shaders/Shader.hpp"

class Object {
private:
	unsigned int VAO;
	unsigned int diffuse, specular;

public:
	float shininess = 64.0f;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

private:
	void updateTransforms();

public:
	Object(std::vector<float>& vertices, unsigned int diffuse, unsigned int specular);
	Object(unsigned int VAO, unsigned int diffuse, unsigned int specular);

	void update(Shader* shader);

public:
	void setPosition(glm::vec3 pos);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
};

class LightCube : public Object {
public:
	glm::vec3 color = glm::vec3(1.0f);

public:
	LightCube(unsigned int VAO) : Object(VAO, NULL, NULL) {};
};

#endif // !CUBE_HPP
