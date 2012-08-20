#include "core/game/game.h"

class MyGame	: public loki::game::LkGame
{
public:
	MyGame();
	~MyGame();

private:
	virtual void PreInit();
	virtual bool Init();
	virtual void PostInit();		// Called when Init() return true.
	virtual void PostInitFail();	// Called when Init() returns false.

	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();

	virtual void PreShutdown();
	virtual void Shutdown();
	virtual void PostShutdown();

	virtual void PreLevelLoad();
	virtual void PostLevelLoad();
};