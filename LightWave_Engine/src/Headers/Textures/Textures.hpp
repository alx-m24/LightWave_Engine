//#pragma once
#ifndef TEXURE_H
#define TEXURE_H

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

unsigned int loadTexture(char const* path);
unsigned int TextureFromFile(const char* path, const std::string& directory);
#endif // TEXTURE_H