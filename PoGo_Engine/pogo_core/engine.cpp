#include "engine.h"

#include "application_listener.h"

namespace Pogo
{
	Engine::Engine(ApplicationListener & listener, EngineConfiguration config) 
		: 
		m_listener(listener), 
		m_config(config), 
		m_window(config)
	{
	}

	Engine::~Engine()
	{
	}
}