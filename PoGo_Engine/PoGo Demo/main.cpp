#include "engine.h"
#include "game.h"



int main(int /*argc*/, char ** /*argv*/)
{
	Pogo::EngineConfiguration config;
	config.width = 1080;
	config.height = 1920;
	config.title = "Test";

	Game game;
	Pogo::Engine engine(game, config);
	engine.Run();
}