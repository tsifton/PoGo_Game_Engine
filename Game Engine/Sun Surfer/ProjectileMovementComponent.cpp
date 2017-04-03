#include "ProjectileMovementComponent.h"



bool ProjectileMovementComponent::Update(Entity & entity, float dt)
{
	entity.worldSpace.position += +entity.worldSpace.lookDirection * m_velocity.z * dt;
	return true;
}

void ProjectileMovementComponent::SetSpeed(float speed)
{
	m_velocity = glm::vec3(speed, 0.0f, speed);
}
