#ifndef PERSPECTIVE_H_
#define PERSPECTIVE_H_

#pragma warning(push)
#pragma warning(disable: 4201)
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

class Perspective
{
public:
	void SetPerspective(float vertFoV, float aspect, float znear, float zfar);
	void SetAspect(float aspect);
	void SetFoV(float vertFov);

	glm::mat4 GetPerspective();

private:
	float m_vertFoV;
	float m_aspect;
	float m_zNear;
	float m_zFar;
};
#endif // !PERSPECTIVE_H_
