#ifndef LIGHT_H_
#define LIGHT_H_

#include "GraphicsObject.h"

#pragma warning(push)
#pragma warning(disable:4201)
#include "vec3.hpp"
#pragma warning(pop)

class Light : public GraphicsObject
{
public:
	Light()
	{

	}

	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : ambient(ambient), diffuse(diffuse), specular(specular)
	{

	}

public:
	void SetAmbientColor(glm::vec3 ambientColor) { ambient = ambientColor; }
	void SetDiffuseColor(glm::vec3 diffuseColor) { diffuse = diffuseColor; }
	void SetSpecularColor(glm::vec3 specularColor) { specular = specularColor; }

	glm::vec3 GetAmbientColor() { return ambient; }
	glm::vec3 GetDiffuseColor() { return diffuse; }
	glm::vec3 GetSpecularColor() { return specular; }

	void SetDirection(glm::vec3 direction) { m_direction = direction; }
	void SetAttenuation(float attenuation) { m_attenuation = attenuation; }
	void SetCutoffAngle(float cutoffAngle) { m_cutoffAngle = cutoffAngle; }

	glm::vec3 GetDirection() { return m_direction; }
	float GetAttenuation() { return m_attenuation; }
	float GetCutoffAngle() { return m_cutoffAngle; }

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	glm::vec3 m_direction;
	float m_attenuation;
	float m_cutoffAngle;
};

#endif // !LIGHT_H_