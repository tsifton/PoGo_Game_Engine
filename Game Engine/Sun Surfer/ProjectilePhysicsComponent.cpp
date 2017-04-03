#include "ProjectilePhysicsComponent.h"

#include "CollisionDetection.h"

#include "MovementComponent.h"
#include "GravityComponent.h"
#include "CollisionComponent.h"

bool ProjectilePhysicsComponent::Update(Entity & entity, float dt)
{
	MovementComponent* movement = GetSiblingComponent<MovementComponent>();
	if (movement)
	{
		GravityComponent* gravity = GetSiblingComponent<GravityComponent>();
		if (gravity)
		{
			glm::vec3& velocity = movement->Velocity();
			velocity += gravity->Acceleration() * dt;
			entity.worldSpace.position.y += velocity.y * dt;

			CollisionComponent* collision = GetSiblingComponent<CollisionComponent>();
			if (collision)
			{
				RaycastHit floor;
				glm::vec3 rayOrigin = glm::vec3(entity.worldSpace.position.x, entity.worldSpace.position.y + collision->GetHeight() / 2, entity.worldSpace.position.z);
				glm::vec3 rayDirection = -entity.modelSpace.upDirection;
				if (CollisionDetection::RayCast(rayOrigin, rayDirection, floor, collision->GetHeight() / 2))
				{
					velocity = movement->Velocity();
					entity.worldSpace.position = floor.point;
					velocity = glm::vec3();
					gravity->SetIsFalling(false);
				}
				else gravity->SetIsFalling(true);

				RaycastHit ceiling;
				rayOrigin = glm::vec3(entity.worldSpace.position.x, entity.worldSpace.position.y + collision->GetHeight() / 2, entity.worldSpace.position.z);
				rayDirection = entity.modelSpace.upDirection;
				if (CollisionDetection::RayCast(rayOrigin, rayDirection, ceiling, collision->GetHeight() / 2))
				{
					velocity = movement->Velocity();
					if (velocity.y > 0.0f) velocity.y = 0.0f;
				}

				RaycastHit wall;
				rayOrigin = glm::vec3(entity.worldSpace.position.x, entity.worldSpace.position.y + collision->GetHeight() / 2, entity.worldSpace.position.z);
				rayDirection = entity.worldSpace.lookDirection;
				if (CollisionDetection::RayCast(rayOrigin, rayDirection, wall, collision->GetHeight() / 2))
				{
					velocity = movement->Velocity();
					velocity = glm::vec3(0.0f, velocity.y * .95, 0.0f);
					if (velocity.y > 0.0f) velocity.y = 0.0f;
				}
			}
		}

	}
	return true;
}