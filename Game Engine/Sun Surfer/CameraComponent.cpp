#include "CameraComponent.h"


bool CameraComponent::Update(Entity & entity, float /*m_dt*/)
{
	/*
	Get Camera's position based on the location of the entity
	that the camera is following, plus some offset.
	*/
	glm::vec3 objPosition = entity.worldSpace.position;
	glm::vec3 offset = glm::vec3(entity.worldSpace.yawMatrix * entity.worldSpace.pitchMatrix * glm::vec4(m_offset, 1.0f));
	glm::vec3 cameraPosition = objPosition + offset + m_lookAtPointOffset;

	/*
	Get Camera's look at point based on the location and look-at direction
	of the entity that the camera is following, plus some offset.
	*/
	glm::vec3 lookAtPoint = objPosition + m_lookAtPointOffset;
	glm::vec3 lookDir = entity.worldSpace.lookDirection;
	glm::vec3 lookAtDirection = glm::vec3(lookDir.x, m_inFirstPerson ? lookDir.y : 0.0f, lookDir.z);

	m_worldToViewMatrix = glm::lookAt(cameraPosition, lookAtPoint + lookAtDirection, entity.modelSpace.upDirection);
	return true;
}

void CameraComponent::SetCameraOffset(glm::vec3 offset)
{
	m_inFirstPerson = (offset == glm::vec3(0.0f, 0.0f, 0.0f));
	m_offset = offset;
}

void CameraComponent::SetLookAtPointOffset(glm::vec3 offset)
{
	m_lookAtPointOffset = offset;
}

void CameraComponent::Zoom(int numSteps)
{
	// Get Change in Y and Z Offset
	float deltaY = -numSteps / 2.0f;
	float deltaZ = numSteps * 1.0f;

	// Add the changes to the previous offset values and clamp them
	// to prevent from zooming in or out too far.
	float yOffset = glm::clamp(m_offset.y + deltaY, 0.0f, 100.0f);
	float zOffset = glm::clamp(m_offset.z + deltaZ, -200.0f, 0.0f);

	SetCameraOffset(glm::vec3(m_offset.x, yOffset, zOffset));
	/*printf("Camera YOffset: %f\n", m_offset.y);
	printf("Camera ZOffset: %f\n", m_offset.z);*/
}

bool CameraComponent::InFirstPerson()
{
	return m_inFirstPerson;
}

glm::mat4 CameraComponent::GetWorldToViewMatrix()
{
	return m_worldToViewMatrix;
}
