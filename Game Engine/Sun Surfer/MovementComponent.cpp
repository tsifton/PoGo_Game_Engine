#include "MovementComponent.h"

#include "KeyboardComponent.h"
#include "CollisionComponent.h"
#include "CollisionDetection.h"
#include "PhysicsComponent.h"
#include "GravityComponent.h"

bool MovementComponent::Initialize()
{
	KeyboardComponent* keyboard = GetSiblingComponent<KeyboardComponent>();
	if (keyboard) keyboard->AddKeys("WASD");
	return true;
}

void MovementComponent::SetForwardSpeed(float speed)
{
	m_velocity.z = speed;
}

void MovementComponent::SetBackwardSpeed(float speed)
{
	m_velocity.z = speed;
}

void MovementComponent::SetStrafeSpeed(float speed)
{
	m_velocity.x = speed;
}

void MovementComponent::SetYawSpeed(float speed)
{
	m_yawSpeed = speed;
}

void MovementComponent::SetPitchSpeed(float speed)
{
	m_pitchSpeed = speed;
}

bool MovementComponent::Update(Entity& entity, float dt)
{
	glm::mat4 yawMatrix	  = glm::rotate(glm::mat4(), glm::radians(entity.worldSpace.yawAngle), entity.modelSpace.upDirection);		// converts to world space
	glm::mat4 pitchMatrix = glm::rotate(glm::mat4(), glm::radians(entity.worldSpace.pitchAngle), entity.modelSpace.rightDirection);	// converts to world space

	entity.worldSpace.yawMatrix   = yawMatrix;
	entity.worldSpace.pitchMatrix = pitchMatrix;

	glm::vec3 lookWorld  = glm::normalize( glm::vec3( yawMatrix * pitchMatrix * glm::vec4( entity.modelSpace.lookDirection,	    0.0f ) ) );		// Affect look direction by yaw and pitch rotations
	glm::vec3 rightWorld = glm::normalize( glm::vec3( yawMatrix	* 			    glm::vec4( entity.modelSpace.rightDirection,	0.0f ) ) );		// Affect right direction by the yaw rotation
	glm::vec3 upWorld    = glm::normalize( glm::vec3(			  pitchMatrix * glm::vec4( entity.modelSpace.upDirection,		0.0f ) ) );		// Affect up direction by the pitch rotation

	entity.worldSpace.lookDirection = lookWorld;
	entity.worldSpace.upDirection = upWorld;
	entity.worldSpace.rightDirection = rightWorld;

	KeyboardComponent* keyboard = GetSiblingComponent<KeyboardComponent>();
	if (keyboard != nullptr)
	{
		glm::vec3 position = entity.worldSpace.position;
		float initialY = position.y;

		CollisionComponent* collision = GetSiblingComponent<CollisionComponent>();
		if (collision)
		{
			if (keyboard->KeyIsDown('W'))
			{
				RaycastHit wall;
				glm::vec3 rayOrigin = glm::vec3(entity.worldSpace.position.x, entity.worldSpace.position.y + collision->GetHeight() / 2, entity.worldSpace.position.z);
				glm::vec3 rayDirection = glm::normalize(glm::vec3(entity.worldSpace.lookDirection.x, 0.0f, entity.worldSpace.lookDirection.z));
				if (!CollisionDetection::RayCast(rayOrigin, rayDirection, wall, collision->GetDepth())) position += +lookWorld * m_velocity.z * dt;
			}

			if (keyboard->KeyIsDown('A'))
			{
				RaycastHit wall;
				glm::vec3 rayOrigin = glm::vec3(entity.worldSpace.position.x, entity.worldSpace.position.y + collision->GetHeight() / 2, entity.worldSpace.position.z);
				glm::vec3 rayDirection = glm::normalize(glm::vec3(-entity.worldSpace.rightDirection.x, 0.0f, -entity.worldSpace.rightDirection.z));
				if (!CollisionDetection::RayCast(rayOrigin, rayDirection, wall, collision->GetWidth())) position += -rightWorld * m_velocity.x * dt;
			}

			if (keyboard->KeyIsDown('S'))
			{
				RaycastHit wall;
				glm::vec3 rayOrigin = glm::vec3(entity.worldSpace.position.x, entity.worldSpace.position.y + collision->GetHeight() / 2, entity.worldSpace.position.z);
				glm::vec3 rayDirection = glm::normalize(glm::vec3(-entity.worldSpace.lookDirection.x, 0.0f, -entity.worldSpace.lookDirection.z));
				if (!CollisionDetection::RayCast(rayOrigin, rayDirection, wall, collision->GetDepth())) position += -lookWorld  * m_velocity.z * dt;
			}

			if (keyboard->KeyIsDown('D'))
			{
				RaycastHit wall;
				glm::vec3 rayOrigin = glm::vec3(entity.worldSpace.position.x, entity.worldSpace.position.y + collision->GetHeight() / 2, entity.worldSpace.position.z);
				glm::vec3 rayDirection = glm::normalize(glm::vec3(entity.worldSpace.rightDirection.x, 0.0f, entity.worldSpace.rightDirection.z));
				if (!CollisionDetection::RayCast(rayOrigin, rayDirection, wall, collision->GetWidth())) position += +rightWorld * m_velocity.x * dt;
			}

		}
		else
		{
			if (keyboard->KeyIsDown('W')) position += +lookWorld  * m_velocity.z * dt;
			if (keyboard->KeyIsDown('A')) position += -rightWorld * m_velocity.x * dt;
			if (keyboard->KeyIsDown('S')) position += -lookWorld  * m_velocity.z * dt;
			if (keyboard->KeyIsDown('D')) position += +rightWorld * m_velocity.x * dt;
		}
		
		GravityComponent* gravityComponent = GetSiblingComponent<GravityComponent>();
		if (gravityComponent)
			position.y = initialY;
		entity.worldSpace.position = position;
	}
	return true;
}

void MovementComponent::Yaw(int dx)
{
	float yawAngle = m_owner->worldSpace.yawAngle;
	yawAngle += dx * m_yawSpeed;
	/*printf("YawAngle: %f\n", yawAngle);*/
	m_owner->worldSpace.yawAngle = yawAngle;
}

void MovementComponent::Pitch(int dy)
{
	float pitchAngle = m_owner->worldSpace.pitchAngle;
	pitchAngle = glm::clamp(pitchAngle + dy * m_pitchSpeed, -60.0f, 80.0f);
	/*printf("PitchAngle: %f\n", pitchAngle);*/
	m_owner->worldSpace.pitchAngle = pitchAngle;
}

glm::vec3& MovementComponent::Velocity()
{
	return m_velocity;
}
