#include "CollidableObject.h"



void CollidableObject::BuildModelToWorldMatrix(glm::vec3 position, glm::mat4 rotateMat, glm::vec3 scale)
{
	glm::mat4 translateMat = glm::translate(glm::mat4(), position);
	glm::mat4 scaleMat = glm::scale(glm::mat4(), scale);
	m_modelToWorldMatrix = translateMat * (rotateMat * scaleMat);
}

void CollidableObject::Enable()
{
	m_enabled = true;
}

void CollidableObject::Disable()
{
	m_enabled = false;
}

void CollidableObject::SetMesh(Mesh * mesh)
{
	m_mesh = mesh;
}

void CollidableObject::SetNextNode(CollidableObject * node)
{
	m_nextNode = node;
}

glm::mat4 CollidableObject::GetModelToWorldMatrix() const
{
	return m_modelToWorldMatrix;
}

Mesh * CollidableObject::GetMesh()
{
	return m_mesh;
}

CollidableObject * CollidableObject::GetNextNode() const
{
	return m_nextNode;
}
