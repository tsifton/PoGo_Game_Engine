#ifndef ENGINE_H_
#define ENGINE_H_

#include <string>

#include "ExportHeader.h"
#include "engine_config.h"
#include "window.h"

namespace Pogo
{
	// Forward Declarations
	class ApplicationListener;

	class ENGINE_SHARED Engine
	{
	public:
		Engine(ApplicationListener& listener, EngineConfiguration config);
		~Engine();

		bool Run();

	private:
		bool InitializeWindow();

		ApplicationListener& m_listener;
		EngineConfiguration m_config;
		Window m_window;
	};
}

#endif // !ENGINE_H_