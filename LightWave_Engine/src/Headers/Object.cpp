#include "Object.hpp"

void Object::updateTransforms()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	if (rotation != glm::vec3(0.0f)) {
		glm::vec3 normRotation = glm::normalize(rotation);
		model = glm::rotate(model, glm::length(rotation), normRotation);
	}
	if (scale != glm::vec3(1.0f)) model = glm::scale(model, scale);
}

Object::Object(std::vector<float>& vertices, unsigned int diffuse, unsigned int specular) : diffuse(diffuse), specular(specular)
{
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

Object::Object(unsigned int VAO, unsigned int diffuse, unsigned int specular) : VAO(VAO), diffuse(diffuse), specular(specular) {}

void Object::update(Shader* shader)
{
	if (diffuse != NULL) {
		shader->setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
	}
	if (specular != NULL) {
		shader->setInt("material.specular", 1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);
	}

	shader->setFloat("material.shininess", this->shininess);

	this->updateTransforms();
	shader->setMat4("model", model);
}

void Object::setPosition(glm::vec3 pos)
{
	this->position = pos;
}

void Object::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void Object::setScale(glm::vec3 scale)
{
	this->scale = scale;
}

glm::vec3 Object::getPosition()
{
	return glm::vec3(position);
}

glm::vec3 Object::getRotation()
{
	return glm::vec3(rotation);
}

glm::vec3 Object::getScale()
{
	return glm::vec3(scale);
}
