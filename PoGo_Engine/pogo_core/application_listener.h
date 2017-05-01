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

	protected:
		// Called by the Application before drawing
		virtual void Update() = 0;
		// Called after Update
		virtual void Draw() = 0;
	};
}

#endif // !APPLICATIONLISTENER_H_

