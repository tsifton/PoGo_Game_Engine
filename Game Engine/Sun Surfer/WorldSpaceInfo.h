#ifndef WORLDSPACEINFO_H_
#define WORLDSPACEINFO_H_

#pragma warning(push)
#pragma warning(disable :4201)
#include "vec3.hpp"
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

struct WorldSpaceInfo
{
	glm::vec3 position;
	glm::vec3 scale;

	glm::vec3 lookDirection;
	glm::vec3 upDirection;
	glm::vec3 rightDirection;

	float yawAngle{ 0.0f };
	float pitchAngle{ 0.0f };

	glm::mat4 yawMatrix;
	glm::mat4 pitchMatrix;
};
#endif // !WORLDSPACEINFO_H_
