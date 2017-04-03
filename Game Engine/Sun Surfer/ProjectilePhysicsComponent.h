#ifndef PROJECTILEPHYSICSCOMPONENT_H_
#define PROJECTILEPHYSICSCOMPONENT_H_

#include "PhysicsComponent.h"

class ProjectilePhysicsComponent : public PhysicsComponent
{
public:
	ProjectilePhysicsComponent() {};
	~ProjectilePhysicsComponent() {};

public:
	bool Update(Entity& entity, float dt) override;

};
#endif // !PROJECTILEPHYSICSCOMPONENT_H_
