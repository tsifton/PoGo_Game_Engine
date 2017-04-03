#include "GravityComponent.h"



void GravityComponent::SetAcceleration(float acc)
{
	m_acceleration.y = acc;
}

void GravityComponent::SetIsFalling(bool falling)
{
	m_isFalling = falling;
}

glm::vec3 GravityComponent::Acceleration()
{
	return m_acceleration;
}

bool GravityComponent::IsFalling()
{
	return m_isFalling;
}
