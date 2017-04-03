#ifndef JUMPCOMPONENT_H_
#define JUMPCOMPONENT_H_

#include "Component.h"

class JumpComponent : public Component
{
public:
	JumpComponent() {};
	~JumpComponent() {};

public:
	bool Initialize() override;
	bool Update(Entity& entity, float dt) override;

	void SetJumpSpeed(float speed);

private:
	glm::vec3 m_jumpSpeed;
};
#endif // !JUMPCOMPONENT_H_
