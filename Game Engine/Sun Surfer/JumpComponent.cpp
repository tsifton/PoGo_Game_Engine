#include "JumpComponent.h"

#include "MovementComponent.h"
#include "GravityComponent.h"
#include "KeyboardComponent.h"
#include <Windows.h>

bool JumpComponent::Initialize()
{
	KeyboardComponent* keyboard = GetSiblingComponent<KeyboardComponent>();
	if (keyboard) keyboard->AddKey(VK_SPACE);
	return true;
}

bool JumpComponent::Update(Entity & /*entity*/, float /*m_dt*/)
{
	MovementComponent* movement = GetSiblingComponent<MovementComponent>();
	if (movement)
	{
		GravityComponent* gravity = GetSiblingComponent<GravityComponent>();
		if (gravity)
		{
			if (!gravity->IsFalling())
			{
				KeyboardComponent* keyboard = GetSiblingComponent<KeyboardComponent>();
				if (keyboard->KeyWasPressed(VK_SPACE))
				{
					glm::vec3& velocity = movement->Velocity();
					velocity.y += m_jumpSpeed.y;
				}
			}
		}
	}
	return true;
}

void JumpComponent::SetJumpSpeed(float acc)
{
	m_jumpSpeed = glm::vec3(0.0f, acc, 0.0f);
}
