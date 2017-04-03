#ifndef PHYSICSCOMPONENT_H_
#define PHYSICSCOMPONENT_H_

#include "Component.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent() {};
	~PhysicsComponent() {};

public:
	bool Update(Entity& entity, float dt) override;

};
#endif // !PHYSICSCOMPONENT_H_
