#ifndef WINDOWCONFIG_H_
#define WINDOWCONFIG_H_

#include <string>

namespace Pogo
{
	struct WindowConfiguration
	{
		int width, height;
		std::string title;
	};
}

#endif // !WINDOWCONFIG_H_