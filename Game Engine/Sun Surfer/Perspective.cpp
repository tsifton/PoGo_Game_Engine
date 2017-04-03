#include "Perspective.h"



void Perspective::SetPerspective(float vertFoV, float aspect, float znear, float zfar)
{
	if (vertFoV <= 0) m_vertFoV = 60.0f;
	else m_vertFoV = vertFoV;
	m_aspect = aspect;
	if (znear <= 0) znear = .01f;
	else m_zNear = znear;
	if (zfar <= 0) m_zFar = 100.0f;
	else m_zFar = zfar;
}

void Perspective::SetAspect(float aspect)
{
	m_aspect = aspect;
}

void Perspective::SetFoV(float vertFov)
{
	m_vertFoV = vertFov;
}

glm::mat4 Perspective::GetPerspective()
{
	return glm::perspective(glm::radians(m_vertFoV), m_aspect, m_zNear, m_zFar);
}
