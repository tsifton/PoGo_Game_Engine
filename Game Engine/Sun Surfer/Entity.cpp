#include "Entity.h"
#include "Component.h"

#include "GameLogger.h"


Entity::Entity(const char * name)
{
	if (name) SetName(name);
}

bool Entity::Intialize()
{
	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		Component* component = m_components[i];
		if (component != nullptr)
			component->Init();
	}

	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		Component* component = m_components[i];
		if (component != nullptr)
			component->PostInit();
	}

	GameLogger::Log(MsgType::Info, "Entity::Intialize() - Successfully intialized Entity: \"%s\"!\n", GetName());
	return true;
}

bool Entity::Shutdown()
{
	GameLogger::Log(MsgType::Info, "Entity::Shutdown() - Successfully shutdown Entity: \"%s\"!\n", GetName());
	return true;
}

bool Entity::Update(float dt)
{
	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		Component* component = m_components[i];
		if (component != nullptr && component->IsEnabled())
			component->Update(*this, dt);
	}
	return true;
}

bool Entity::AddComponent(Component * component, const char * name)
{
	if (component == nullptr)
	{
		GameLogger::Log(MsgType::cError, "Entity::AddComponent() - Unable to add Component: \"%s\" to Entity: \"%s\". Component is NULLPTR.\n", m_name, component->GetName());
		return false;
	}

	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		if (m_components[i] == nullptr)
		{
			m_components[i] = component;
			component->SetOwner(this);
			component->SetName(name);
			return true;
		}
	}

	GameLogger::Log(MsgType::cFatalError, "Entity::AddComponent() - Entity: \"%s\" has no room left for components, size of component buffer = %d.\n", m_name, MAX_COMPONENTS);
	return false;
}

bool Entity::ClearComponents()
{
	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		m_components[i] = nullptr;
	}
	return true;
}

void Entity::SetName(const char * name)
{
	strncpy_s(m_name, MAX_NAME_LENGTH, name, _TRUNCATE);
}

std::ostream & Entity::Output(std::ostream & os)
{
	os << "===== Begin Entity : (" << GetName() << ") =====\n";
	const int bufferSize = 100;
	char buffer[bufferSize];
	int counter = 0;
	for (int i = 0; i < MAX_COMPONENTS; ++i)
	{
		Component* component = m_components[i];
		if (component != nullptr)
		{
			sprintf_s(buffer, bufferSize, "   %d : Component (%s)\n",
				counter++, component->GetName());
			os << buffer;
		}
	}
	os << "=====  Done Entity : (" << GetName() << ") =====\n";
	return os;
}

std::ostream & operator<<(std::ostream & os, Entity & entity)
{
	return entity.Output(os);
}
