#pragma once
#ifndef CUBE_HPP
#define CUBDE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "Shaders/Shader.hpp"
#include "Useful.hpp"

class Object {
public:
	float shininess = 64.0f;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 color = glm::vec3(0.0f);

private:
	void updateTransforms();

public:
	Object() = default;
	Object(glm::vec3 position) : position(position) {};
	Object(glm::vec3 position, glm::vec3 rotation) : position(position), rotation(rotation) {};
	Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : position(position), rotation(rotation), scale(scale) {};
	Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float shininess) : position(position), rotation(rotation), scale(scale), shininess(shininess) {};

	void update(Shader& shader);

public:
	void setPosition(glm::vec3 pos);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
};

class Objects : public std::vector<Object> {
private:
	unsigned int VAO;
	unsigned int diffuse, specular;
	unsigned int vertexCount;

public:
	Objects(unsigned int VAO, unsigned int vertexCount, unsigned int diffuse, unsigned int specular);

public:
	void draw(Shader& shader);
};

class LightCube : public Object {
public:
	LightCube() = default;
	LightCube(glm::vec3 color) {
		this->color = color;
	};
};

class Transparent : public Object {
public:
	unsigned int VAO;
	unsigned int diffuse, specular;
	unsigned int vertexCount;

public:
	Transparent(unsigned int VAO, unsigned int vertexCount, unsigned int diffuse, unsigned int specular) : VAO(VAO), vertexCount(vertexCount), diffuse(diffuse), specular(specular) {};

public:
	void draw(Shader& shader);
};

class Transparents : public std::vector<Transparent> {
public:
	void draw(Shader& shader);
};

#endif // !CUBE_HPP