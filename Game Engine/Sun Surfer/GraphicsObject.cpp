#include "GraphicsObject.h"



void GraphicsObject::SetPosition(const glm::vec3& position)
{
	if (m_position != position)
	{
		m_position = position;
		isMatrixDirty = true;
	}
}

void GraphicsObject::SetScale(const glm::vec3& scale)
{
	if (m_scale != scale)
	{
		m_scale = scale;
		isMatrixDirty = true;
	}
}

void GraphicsObject::SetRotation(const glm::vec3& axis, float angle)
{
	if (m_rotationAngle != angle || m_rotationAxis != axis)
	{
		m_rotationAxis = axis;
		m_rotationAngle = angle;
		isMatrixDirty = true;
	}
}

glm::mat4 GraphicsObject::GetModelToWorldMatrix()
{
	if (isMatrixDirty)
	{
		glm::mat4 scaleMat = glm::scale(glm::mat4(), m_scale);
		glm::mat4 rotationMat = glm::rotate(glm::mat4(), glm::radians(m_rotationAngle), m_rotationAxis);
		glm::mat4 translateMat = glm::translate(glm::mat4(), m_position);
		m_modelToWorldMatrix = translateMat * rotationMat * scaleMat;
	}
	return m_modelToWorldMatrix;
}

glm::vec4 GraphicsObject::GetPosition() const
{
	return glm::vec4(m_position, 1.0f);
}

glm::vec3 GraphicsObject::GetScale() const
{
	return m_scale;
}

void GraphicsObject::SetUniformCallback(std::function<void(GraphicsObject*gob, ShaderProgram*shader)> callback)
{
	preDrawCallback = callback;
}

void GraphicsObject::SetSubroutineName(char * name)
{
	subroutineName = name;
}
