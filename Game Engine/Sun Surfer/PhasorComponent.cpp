#include "PhasorComponent.h"

#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "ProjectileMovementComponent.h"

bool PhasorComponent::Initialize()
{
	KeyboardComponent* keyboard = GetSiblingComponent<KeyboardComponent>();
	if (keyboard) keyboard->AddKey('F');
	return true;
}

bool PhasorComponent::Update(Entity & entity, float /*m_dt*/)
{
	KeyboardComponent* keyboard = GetSiblingComponent<KeyboardComponent>();
	if (keyboard)
	{
		if (keyboard->KeyWasReleased('F'))
		{
			if (m_bullet == nullptr)
			{
				m_bullet = m_builder->BuildBullet("Bullet", entity.worldSpace.position, glm::normalize(entity.worldSpace.lookDirection));
				m_bullet->worldSpace.yawMatrix = entity.worldSpace.yawMatrix;
				ProjectileMovementComponent* movement = m_bullet->GetComponentByType<ProjectileMovementComponent>();
				movement->SetSpeed(75.0f);
			}
			else
			{
				m_bullet->worldSpace.position = entity.worldSpace.position;
				m_bullet->worldSpace.lookDirection = glm::normalize(entity.worldSpace.lookDirection);
				m_bullet->worldSpace.yawMatrix = entity.worldSpace.yawMatrix;
				ProjectileMovementComponent* movement = m_bullet->GetComponentByType<ProjectileMovementComponent>();
				movement->SetSpeed(75.0f);
			}
		}
	}
	return true;
}

void PhasorComponent::AddEntityBuilder(EntityManager* builder)
{
	m_builder = builder;
}
