#ifndef MODELSPACEINFO_H_
#define MODELSPACEINFO_H_

#pragma warning(push)
#pragma warning(disable :4201)
#include "vec3.hpp"
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

struct ModelSpaceInfo
{
	glm::vec3 lookDirection;
	glm::vec3 upDirection;
	glm::vec3 rightDirection;
};
#endif // !MODELSPACEINFO_H_
