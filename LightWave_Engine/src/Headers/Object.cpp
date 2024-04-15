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

void Object::update(Shader& shader)
{
	this->updateTransforms();

	shader.setMat4("model", model);
	shader.setVec3("color", this->color);
	shader.setFloat("material.shininess", this->shininess);
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

Objects::Objects(unsigned int VAO, unsigned int vertexCount, unsigned int diffuse, unsigned int specular) : VAO(VAO), vertexCount(vertexCount), diffuse(diffuse), specular(specular) {};

void Objects::draw(Shader& shader)
{
	if (diffuse != NULL) {
		shader.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
	}
	if (specular != NULL) {
		shader.setInt("material.specular", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);
	}

	glBindVertexArray(VAO);
	for (Object& object : *this) {
		object.update(shader);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
	glBindVertexArray(0);
}

void Transparent::draw(Shader& shader)
{
	this->update(shader);

	if (diffuse != NULL) {
		shader.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
	}
	if (specular != NULL) {
		shader.setInt("material.specular", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
}

void Transparents::draw(Shader& shader)
{
	std::map<float, Transparent*> sorted;
	for (Transparent* temp : *this)
	{
		float distance = glm::length(camera.Position - temp->position);
		sorted[distance] = temp;
	}

	shader.use();

	glDisable(GL_CULL_FACE);
	for (std::map<float, Transparent*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
		it->second->draw(shader);
	}
	glEnable(GL_CULL_FACE);
}
