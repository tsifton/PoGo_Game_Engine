#include "Camera.h"
#include <iostream>


bool Camera::Initialize()
{
	// will make all of these values setable in config
	m_mouseSpeed = 0.75f;
	m_yawAngle = 0.0f;
	m_pitchAngle = 0.0f;

	m_panSpeed = 25.0f;
	m_forwardSpeed = 25.0f;
	m_risingSpeed = 25.0f;

	m_objectOffset = 15.0f;

	m_dt = 0;

	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_viewDirection = glm::vec3(0.0f, -.2, -1.2);
	m_upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	m_ortho = glm::cross(m_viewDirection, m_upDirection);
	return true;
}

bool Camera::Shutdown()
{
	return true;
}

void Camera::Update(float /*m_dt*/)
{
	/*if (ObjectIsAttached())
	{
		glm::vec3 attachedObjectPosition = m_position + (m_viewDirection * m_objectOffset);
		m_attachedObject->translateMat = glm::translate(glm::mat4(), attachedObjectPosition);
	}*/
}

void Camera::AttachObject(GraphicsObject * /*gob*/)
{
	/*if (ObjectIsAttached()) UnattachObject();
	m_attachedObject = gob;
	m_objectDefaultSize = gob->scaleMat;
	m_objectIsAttached = true;*/
}

void Camera::UnattachObject()
{
	/*PrintObjectPosition();
	m_attachedObject->scaleMat = m_objectDefaultSize;
	m_attachedObject = nullptr;
	m_objectIsAttached = false;*/
}

bool Camera::ObjectIsAttached()
{
	return m_objectIsAttached;
}

glm::mat4 Camera::GetWorldToViewMatrix()
{
	if (isPressed[Keys::W] && !isPressed[Keys::S]) m_position += (+m_viewDirection * m_forwardSpeed) * m_dt;
	if (isPressed[Keys::S] && !isPressed[Keys::W]) m_position += (-m_viewDirection * m_forwardSpeed) * m_dt;

	if (isPressed[Keys::A] && !isPressed[Keys::D]) m_position += (-m_ortho * m_panSpeed) * m_dt;
	if (isPressed[Keys::D] && !isPressed[Keys::A]) m_position += (+m_ortho * m_panSpeed) * m_dt;

	if (isPressed[Keys::R] && !isPressed[Keys::F]) m_position += (+m_upDirection * m_risingSpeed) * m_dt;
	if (isPressed[Keys::F] && !isPressed[Keys::R]) m_position += (-m_upDirection * m_risingSpeed) * m_dt;

	for (int i = 0; i < Keys::numEnums; i++)
	{
		isPressed[i] = false;
	}
	return glm::lookAt(m_position, m_position + m_viewDirection, m_upDirection);
}

glm::vec3 Camera::GetPosition()
{
	return m_position;
}

void Camera::ShowCameraInfo()
{
	printf("---------------------------------------\n\"Camera Position\"  - [X: %f Y: %f Z: %f]\n", m_position.x, m_position.y, m_position.z);
	printf("\"Camera Direction\" - [X: %f Y: %f Z: %f]\n---------------------------------------\n", m_viewDirection.x, m_viewDirection.y, m_viewDirection.z);
}

void Camera::MouseRotate(int dx, int dy)
{
	m_yawAngle = static_cast<float>(dx);
	m_pitchAngle = static_cast<float>(dy);
	m_viewDirection = glm::mat3(glm::rotate(glm::mat4(), glm::radians(-m_yawAngle) * m_mouseSpeed, m_upDirection)) * m_viewDirection;

	m_ortho = glm::cross(m_viewDirection, m_upDirection);
	m_viewDirection = glm::mat3(glm::rotate(glm::mat4(), glm::radians(-m_pitchAngle) * m_mouseSpeed, m_ortho)) * m_viewDirection;
}

void Camera::MoveForward(float dt)
{
	m_dt = dt;
	isPressed[Keys::W] = true;
}

void Camera::MoveLeft(float dt)
{
	m_dt = dt;
	isPressed[Keys::A] = true;
}

void Camera::MoveBack(float dt)
{
	m_dt = dt;
	isPressed[Keys::S] = true;
}

void Camera::MoveRight(float dt)
{
	m_dt = dt;
	isPressed[Keys::D] = true;
}

void Camera::MoveUp(float dt)
{
	m_dt = dt;
	isPressed[Keys::R] = true;
}

void Camera::MoveDown(float dt)
{
	m_dt = dt;
	isPressed[Keys::F] = true;
}

void Camera::PrintObjectPosition()
{
	/*glm::vec3 objectPosition = glm::vec3(m_attachedObject->translateMat * glm::vec4(0, 0, 0, 1));
	printf("---------------------------------------\n\"Unattached Object's New Position\"  - [X: %f Y: %f Z: %f]\n---------------------------------------\n", objectPosition.x, objectPosition.y, objectPosition.z);*/
}
