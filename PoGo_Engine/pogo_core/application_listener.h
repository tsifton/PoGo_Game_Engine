#ifndef APPLICATIONLISTENER_H_
#define APPLICATIONLISTENER_H_

#include "ExportHeader.h"

namespace Pogo
{
	class ENGINE_SHARED ApplicationListener
	{
	public:
		// Virtual destructor for base cleanup
		virtual ~ApplicationListener() {};

		// Called by the Engine before drawing
		virtual void Update() = 0;
		// Called after Update
		virtual void Draw() = 0;
	};
}

#endif // !APPLICATIONLISTENER_H_

