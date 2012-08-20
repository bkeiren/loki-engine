#include "core/engine.h"
#include "game.h"

int main( int argc, char** argv )
{
	loki::game::LkGame* game = new MyGame();
	loki::LkEngine* engine = new loki::LkEngine(game);
	engine->SetFrameRateCap(60);
	engine->Go(argc, argv);
	delete engine;
	engine = NULL;
	// No need to delete game, engine will delete it at shutdown.
	return 0;
}