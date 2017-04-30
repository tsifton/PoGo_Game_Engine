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
	Material(glm::vec3 ambient = glm::vec3(), glm::vec3 diffuse = glm::vec3(), glm::vec3 specular = glm::vec3(), float shininess = 32.0f) : 
		m_ambient(ambient),
		m_diffuse(diffuse),
		m_specular(specular),
		m_shininess(shininess) {}

public:
	void SetEmissive(const glm::vec3& emissive) { m_emissive = emissive; }
	void SetAmbient(const glm::vec3& ambient) { m_ambient = ambient; }
	void SetDiffuse(const glm::vec3& diffuse) { m_diffuse = diffuse; }
	void SetSpecular(const glm::vec3& specular) { m_specular = specular; }
	void SetShininess(float shininess) { m_shininess = shininess; }

	glm::vec3 GetEmissive() { return m_emissive; }
	glm::vec3 GetAmbient() { return m_ambient; }
	glm::vec3 GetDiffuse() { return m_diffuse; }
	glm::vec3 GetSpecular() { return m_specular; }
	float	  GetShininess() { return m_shininess; }

	void AddLightIndex(int index);
	void AddLightIndices(int indices[], int numValues);
	std::vector<int> GetLightIndices() { return lightIndices; }

	void AddTexture(Texture::SharedPtr texture);
	std::vector<Texture::SharedPtr> GetTextures() { return textures; }

private:
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float	  m_shininess;
	glm::vec3 m_emissive;

	std::vector<Texture::SharedPtr> textures;
	std::vector<int> lightIndices;
};

#endif // !MATERIAL_H_