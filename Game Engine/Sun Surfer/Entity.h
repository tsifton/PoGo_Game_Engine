#ifndef ENTITY_H_
#define ENTITY_H_

#include <ostream>
#include "ModelSpaceInfo.h"
#include "WorldSpaceInfo.h"

class Component;
class Entity
{
public:
	Entity(const char* name = nullptr);
	~Entity() {};

public:
	bool	Intialize();
	bool	Shutdown();
	bool	Update(float dt);

	bool	AddComponent(Component* component, const char* name);
	bool	ClearComponents();
	template <class T> T* GetComponentByType() const;

	void SetName(const char* name);
	const char* GetName() const { return m_name; };

	std::ostream& Output(std::ostream& os);

public:
	ModelSpaceInfo modelSpace;
	WorldSpaceInfo worldSpace;

protected:
	static const int MAX_COMPONENTS = 20;
	static const int MAX_NAME_LENGTH = 32;
	Component*	m_components[MAX_COMPONENTS] { nullptr };
	char		m_name[MAX_NAME_LENGTH]	{ 0 };
};

template<class ComponentType>
inline ComponentType * Entity::GetComponentByType() const
{
	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		Component* component = m_components[i];
		if (component != nullptr)
			if (component->IsEnabled())
				if (ComponentType* cmp = dynamic_cast<ComponentType*>(m_components[i]))
					return cmp;
	}
	return nullptr;
}

std::ostream& operator<<(std::ostream& os, Entity& entity);

#endif // !ENTITY_H_