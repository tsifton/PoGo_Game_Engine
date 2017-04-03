#ifndef GRAPHICSOBJECT_H_
#define GRAPHICSOBJECT_H_

#include "Mesh.h"
#include "ShaderProgram.h"
#include "LightInfo.h"
#include "Material.h"
#include "TextureLoader.h"

#include <functional>

#pragma warning(push)
#pragma warning(disable :4201)
#include "vec3.hpp"
#include "mat4x4.hpp"
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

class GraphicsObject
{

public:
	void SetPosition(const glm::vec3& position);
	void SetScale(const glm::vec3& scale);
	void SetRotation(const glm::vec3& axis, float angle);

	glm::mat4 GetModelToWorldMatrix();
	glm::vec4 GetPosition() const;
	glm::vec3 GetScale() const;

	void SetUniformCallback(std::function <void(GraphicsObject* gob, ShaderProgram* shader)> callback);
	void SetSubroutineName(char* name);

	std::function <void(GraphicsObject* gob, ShaderProgram* shader)> GetUniformCallback() { return preDrawCallback; }
	char* GetSubroutineName() { return subroutineName; }

public:
	GLuint shaderID;
	bool   isEnabled{ true };

	Material  material;
	Mesh* mesh;
	GraphicsObject* nextNode{ nullptr };

private:
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotationAxis;
	float     m_rotationAngle { 0 };
	bool isMatrixDirty = false;

	char*	  subroutineName;
	std::function <void(GraphicsObject* gob, ShaderProgram* shader)> preDrawCallback{ nullptr };
	glm::mat4 m_modelToWorldMatrix;
};

#endif // !GRAPHICSOBJECT_H_