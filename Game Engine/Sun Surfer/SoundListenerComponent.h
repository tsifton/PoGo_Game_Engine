#ifndef SOUNDLISTENERCOMPONENT_H_
#define SOUNDLISTENERCOMPONENT_H_

#include "Component.h"

#pragma warning(push)
#pragma warning(disable: 4458)
#include "yse.hpp"
#pragma warning(pop)

class SoundListenerComponent : public Component
{
public:
	SoundListenerComponent() {};
	~SoundListenerComponent() {};

public:
	bool Update(Entity& entity, float dt);
};
#endif // !SOUNDLISTENERCOMPONENT_H_
