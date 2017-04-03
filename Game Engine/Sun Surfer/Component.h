#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "Entity.h"
#include "GameLogger.h"

class Component
{
public:
	Component() {};
	~Component() {};

public:
	bool Init();
	bool PostInit();

	virtual bool Update(Entity& /*entity*/, float /*m_dt*/)	{ return true; }

	void		SetName(const char* name);
	const char* GetName() const				{ return m_name; }
	void		SetOwner(Entity* owner)		{ m_owner = owner; }
	void		Enable()					{ m_enabled = true; }
	void		Disable()					{ m_enabled = false; }
	bool		IsEnabled()					{ return m_enabled; }
	bool		IsDisabled()				{ return !m_enabled; }

protected:
	virtual bool Initialize() { return true; }
	virtual bool PostInitialize() { return true; }

	template <class T> T* GetSiblingComponent()
	{
		if (m_owner == nullptr) 
		{ 
			GameLogger::Log(MsgType::cFatalError, "Component::GetSiblingComponent() - Unable to retrieve component from Entity. Entity is NULLPTR!\n");
			return nullptr;
		}
		return m_owner->GetComponentByType<T>();
	}

protected:
	static const int MAX_NAME_LENGTH = 32;

	Entity* m_owner{ nullptr };
	char	m_name[MAX_NAME_LENGTH]{ 0 };
	bool	m_enabled{ true };

};

#endif // !COMPONENT_H_