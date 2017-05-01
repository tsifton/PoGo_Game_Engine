#ifndef GAME_H_
#define GAME_H_

#include "application_listener.h"

class Game : public Pogo::ApplicationListener
{
public:
	Game();
	~Game();

	// Inherited via ApplicationListener
	void Update() override;
	void Draw() override;
};

#endif // !GAME_H_