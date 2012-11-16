#pragma once

#ifndef GAME_H
#define GAME_H

namespace loki
{

namespace game
{

class LkGame
{
	friend class LokiEngine;
public:
	LkGame();
	virtual ~LkGame() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Set this data in your c-tor.
	//////////////////////////////////////////////////////////////////////////
	const int32 GameVersionMajor;
	const int32 GameVersionMinor;
	const int32 GameVersionBuild;
	const char* GameName;
protected:
	virtual void PreInit() = 0;
	virtual bool Init() = 0;
	virtual void PostInit() = 0;		// Called when Init() return true.
	virtual void PostInitFail() = 0;	// Called when Init() returns false.

	virtual void PreUpdate() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;

	virtual void PreShutdown() = 0;
	virtual void Shutdown() = 0;
	virtual void PostShutdown() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Called before level loading.
	//////////////////////////////////////////////////////////////////////////
	virtual void PreLevelLoad() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Called after level loading.
	//////////////////////////////////////////////////////////////////////////
	virtual void PostLevelLoad() = 0;
};

}

}

#endif