#ifndef PHASORCOMPONENT_H_
#define PHASORCOMPONENT_H_

#include "Component.h"
#include "EntityManager.h"

class PhasorComponent : public Component
{
public:
	PhasorComponent() {};
	~PhasorComponent() {};

public:
	bool Initialize() override;
	bool Update(Entity& entity, float dt) override;

	void AddEntityBuilder(EntityManager* builder);

private:
	float m_fireRate;

	EntityManager* m_builder;
	Entity* m_bullet{ nullptr };
};
#endif // !PHASORCOMPONENT_H_
