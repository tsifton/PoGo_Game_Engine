#ifndef MOUSECOMPONENT_H_
#define MOUSECOMPONENT_H_

#include "Component.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#include "vec2.hpp"
#pragma warning(pop)

class MouseComponent : public Component
{
public:
	MouseComponent();
	~MouseComponent();

public:
	void MoveMouse(int dx, int dy);
	void ScrollMouse(int numSteps);

};
#endif // !MOUSECOMPONENT_H_
