#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include "Mesh.hpp"
#include "Shaders/Shader.hpp"

class Model {
public:
	Model(std::string path);

public:
	float shininess = 64.0f;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	void draw(Shader* shader);

private:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;

private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif // !MODEL_H
