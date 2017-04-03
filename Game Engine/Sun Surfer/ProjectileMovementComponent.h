#ifndef PROJECTILEMOVEMENTCOMPONENT_H_
#define PROJECTILEMOVEMENTCOMPONENT_H_

#include "MovementComponent.h"

#pragma warning(push)
#pragma warning(disable :4201)
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

class ProjectileMovementComponent : public MovementComponent
{
public:
	ProjectileMovementComponent() {};
	~ProjectileMovementComponent() {};

public:
	bool Update(Entity& entity, float dt) override;
	
	void SetSpeed(float speed);

};
#endif // !PROJECTILEMOVEMENTCOMPONENT_H_
