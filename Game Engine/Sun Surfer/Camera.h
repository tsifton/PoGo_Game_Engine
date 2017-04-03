#ifndef CAMERA_H_
#define CAMERA_H_

#pragma warning(push)
#pragma warning(disable: 4201)
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

#include "GraphicsObject.h"

enum Keys
{
	W, A, S, D, R, F,

	numEnums // always last
};

class Camera
{
public:
	bool Initialize();
	bool Shutdown();

	void Update(float dt);

	void AttachObject(GraphicsObject* gob);
	void UnattachObject();
	bool ObjectIsAttached();

	glm::mat4 GetWorldToViewMatrix();
	glm::vec3 GetPosition();

	void ShowCameraInfo();

	void MouseRotate(int dx, int dy);
	void MoveForward(float dt);
	void MoveLeft(float dt);
	void MoveBack(float dt);
	void MoveRight(float dt);
	void MoveUp(float dt);
	void MoveDown(float dt);

private:
	void PrintObjectPosition();

private:
	glm::vec3 m_position;
	glm::vec3 m_viewDirection;
	glm::vec3 m_upDirection;
	glm::vec3 m_ortho;

	float m_mouseSpeed;
	float m_yawAngle;
	float m_pitchAngle;

	float m_panSpeed;
	float m_forwardSpeed;
	float m_risingSpeed;

	glm::mat4 m_objectDefaultSize;
	float m_objectOffset;

	float m_dt;

	GraphicsObject* m_attachedObject{ nullptr };
	bool m_objectIsAttached{ false };

	bool isPressed[Keys::numEnums - 1]{ false };
};
#endif // !CAMERA_H_
