#ifndef MOVEMENTCOMPONENT_H_
#define MOVEMENTCOMPONENT_H_

#include "Component.h"

#pragma warning(push)
#pragma warning(disable :4201)
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

class MovementComponent : public Component
{
public:
	MovementComponent() {};
	~MovementComponent() {};

public:
	bool Initialize() override;

	bool Update(Entity& entity, float dt) override;

	void SetForwardSpeed(float speed);
	void SetBackwardSpeed(float speed);
	void SetStrafeSpeed(float speed);

	void SetYawSpeed(float speed);
	void SetPitchSpeed(float speed);

	void Yaw(int dx);
	void Pitch(int dy);

	glm::vec3& Velocity();

protected:
	glm::vec3 m_velocity;

	float m_yawSpeed;
	float m_pitchSpeed;
};
#endif // !MOVEMENTCOMPONENT_H_
