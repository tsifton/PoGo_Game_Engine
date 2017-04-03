#include "MouseComponent.h"

#include "MovementComponent.h"
#include "CameraComponent.h"

MouseComponent::MouseComponent()
{
}


MouseComponent::~MouseComponent()
{
}

void MouseComponent::MoveMouse(int dx, int dy)
{
	MovementComponent* movement = GetSiblingComponent<MovementComponent>();
	if (movement == nullptr) return;
	movement->Yaw(dx);
	movement->Pitch(dy);
}

void MouseComponent::ScrollMouse(int numSteps)
{
	CameraComponent* camera = GetSiblingComponent<CameraComponent>();
	if (camera == nullptr) return;
	camera->Zoom(numSteps);
}
