#ifndef CAMERACOMPONENT_H_
#define CAMERACOMPONENT_H_

#include "Component.h"

#pragma warning(push)
#pragma warning(disable :4201)
#include "vec3.hpp"
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

class CameraComponent : public Component
{
public:
	CameraComponent() {};
	~CameraComponent() {};

public:
	bool Update(Entity& entity, float dt) override;

	void SetCameraOffset(glm::vec3 offset);
	void SetLookAtPointOffset(glm::vec3 offset);

	void Zoom(int numSteps);
	bool InFirstPerson();

	glm::mat4 GetWorldToViewMatrix();

private:
	glm::vec3 m_offset;
	glm::vec3 m_lookAtPointOffset;

	glm::mat4 m_worldToViewMatrix;

	bool m_inFirstPerson = true;
};
#endif // !CAMERACOMPONENT_H_
