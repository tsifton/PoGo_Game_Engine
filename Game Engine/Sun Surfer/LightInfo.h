#pragma once

#pragma warning(push)
#pragma warning(disable :4201)
#include "vec3.hpp"
#include "vec4.hpp"
#pragma warning(pop)

class LightInfo
{
public:
	LightInfo(	glm::vec4 position = glm::vec4(),
				glm::vec3 diffuseColor = glm::vec3(),
				glm::vec3 ambientColor = glm::vec3(),
				glm::vec3 specularColor = glm::vec3())
		: 
				position(position), 
				diffuseColor(diffuseColor),
				ambientColor(ambientColor),
				specularColor(specularColor){}
	~LightInfo() {};

public:
	glm::vec4 position;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
};

