#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <vector>

#pragma warning(push)
#pragma warning(disable:4201)
#include "vec3.hpp"
#pragma warning(pop)

#include <GL\glew.h>
#include "Texture.h"

class Material
{
public:
	Material()
	{
	}

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
	{
	}

public:
	void AddLightIndex(int index);
	void AddLightIndices(int indices[], int numValues);
	std::vector<int> GetLightIndices() { return lightIndices; }

	void AddTexture(Texture::SharedPtr texture);
	std::vector<Texture::SharedPtr> GetTextures() { return textures; }

public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float	  shininess;
	glm::vec3 emissive;

private:
	std::vector<Texture::SharedPtr> textures;
	std::vector<int> lightIndices;
};

#endif // !MATERIAL_H_