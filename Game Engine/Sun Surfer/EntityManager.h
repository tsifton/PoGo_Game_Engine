#ifndef ENTITYMANAGER_H_
#define ENTITYMANAGER_H_

#pragma warning(push)
#pragma warning(disable :4201)
#include "gtc\matrix_transform.hpp"
#pragma warning(pop)

#include "MouseComponent.h"
#include "CameraComponent.h"

class Entity;
class EntityManager
{
public:
	EntityManager() {};
	~EntityManager() {};

public:
	bool Update(float dt);

	glm::mat4		 GetWorldToViewMatrix();
	MouseComponent*	 GetMouse();
	CameraComponent* GetCamera();

	bool AddEntity(Entity* entity);

	Entity* BuildDebugCamera(const char* name);
	Entity* BuildPlayer(const char* name);
	Entity* BuildBullet(const char* name, glm::vec3 position, glm::vec3 velocity);
	Entity* BuildDargon(const char* name);
	Entity* BuildStereo(const char* name);
	Entity* BuildFirepit(const char* name);

private:
	static const int MAX_ENTITIES = 104;
	Entity* m_entities[MAX_ENTITIES]{ nullptr };
};

#endif // !ENTITYMANAGER_H_