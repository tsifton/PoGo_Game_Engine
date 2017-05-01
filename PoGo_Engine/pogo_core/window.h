#ifndef WINDOW_H_
#define WINDOW_H_

#include "ExportHeader.h"
#include "engine_config.h"

namespace Pogo
{
	class ENGINE_SHARED Window
	{
	public:
		Window(WindowConfiguration config);
		~Window();

	private:
		WindowConfiguration m_config;
	};
}

#endif // !WINDOW_H_