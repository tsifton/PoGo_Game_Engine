#ifndef COLLISIONCOMPONENT_H_
#define COLLISIONCOMPONENT_H_

#include "Component.h"
#include "CollidableObject.h"

class CollisionComponent : public Component
{
public:
	CollisionComponent() {};
	~CollisionComponent() {};

public:
	bool Initialize() override;
	bool Update(Entity& entity, float dt) override;

	void SetSceneFilepath(const char * filepath);
	void BuildModelToWorldMatrix(glm::vec3 position, glm::mat4 rotate, glm::vec3 scale);

	float GetWidth();
	float GetHeight();
	float GetDepth();

private:
	char m_sceneFilepath[100];
	CollidableObject m_collidable;
};
#endif // !COLLISIONCOMPONENT_H_
