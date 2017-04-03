#ifndef GRAVITYCOMPONENT_H_
#define GRAVITYCOMPONENT_H_

#include "Component.h"

class GravityComponent : public Component
{
public:
	GravityComponent() {};
	~GravityComponent() {};

public:
	void SetAcceleration(float acc);
	void SetIsFalling(bool falling);

	glm::vec3	Acceleration();
	bool		IsFalling();

private:
	glm::vec3	m_acceleration;
	bool		m_isFalling{ true };

};
#endif // !GRAVITYCOMPONENT_H_
