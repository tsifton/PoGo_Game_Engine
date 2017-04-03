#include "Component.h"

#include <string>


bool Component::Init()
{
	bool initialized = Initialize();

	if (initialized)	GameLogger::Log(MsgType::Info, "Component::Init() - Successfully initialized Component: \"%s\" of Entity: \"%s\"!\n", m_name, m_owner->GetName());
	else				GameLogger::Log(MsgType::Error, "Component::Init() - Failed to initialize Component: \"%s\" of Entity: \"%s\"!\n", m_name, m_owner->GetName());
	return initialized;
}

bool Component::PostInit()
{
	bool initialized = PostInitialize();

	if (initialized)	GameLogger::Log(MsgType::Info, "Component::PostInit() - Successfully initialized Component: \"%s\" of Entity: \"%s\"!\n", m_name, m_owner->GetName());
	else				GameLogger::Log(MsgType::Error, "Component::PostInit() - Failed to initialize Component: \"%s\" of Entity: \"%s\"!\n", m_name, m_owner->GetName());
	return initialized;
}

void Component::SetName(const char * name)
{
	strncpy_s(m_name, MAX_NAME_LENGTH, name, _TRUNCATE);
}
