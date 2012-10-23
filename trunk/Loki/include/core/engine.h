#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include <Windows.h>
#include <WinDef.h>

#include <iostream>
#include <ctime>

#include <list>
#include "core/commandlineparsing.h"
#include "core/statemanager.h"

#include "util/jobmanager/jobmanager.h"

#include "util/clock/clock.h"

namespace loki
{

//////////////////////////////////////////////////////////////////////////
// Loki engine version values. Can be accessed from the loki namespace.
// The constants themselves reside in engine.cpp so that they can not
// be removed or altered from the header file.
//////////////////////////////////////////////////////////////////////////
extern const int32 VersionMajor;
extern const int32 VersionMinor;
extern const int32 VersionBuild;
extern const char* VersionName;
extern const char* VersionType;

namespace game
{
	class LkGame;
}

class LkWindow;

//////////////////////////////////////////////////////////////////////////
// Static functions
//////////////////////////////////////////////////////////////////////////
class LkEngine
{
public:
	LkEngine( game::LkGame* _Game );
	~LkEngine();

	void Go( int32 argc, char** argv );

	LkWindow* GetWindow() const;
	game::LkGame* GetGame();

	//////////////////////////////////////////////////////////////////////////
	// In seconds.
	f32 GetFrameTime() const;

	//////////////////////////////////////////////////////////////////////////
	// In seconds. Returns the time that has passed since the engine's Go()
	// function was called.
	f32 GetEngineUpTime() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns the number of frames per second, based on the current frame time.
	f32 GetFrameRate() const;

	//////////////////////////////////////////////////////////////////////////
	// Sets the frame rate cap. If a frame cap of 0 is specified, the cap
	// is disabled. When enabled, the engine will check it's current
	// frame time against the time required for the frame cap and if necessary
	// will sleep for a certain amount of time in order to achieve a frame cap.
	void SetFrameRateCap( uint32 _Cap );
	bool GetFrameCapEnabled() const;

	void ShowBackgroundConsoleWindow() const;
	void HideBackgroundConsoleWindow() const;
	
	void RequestExit();
private:
	LkEngine();

	/*
		ParseArguments parses the command line arguments and stores them in a more easily accessible format in m_CommandLineArguments.
	*/
	void ParseArguments( int32 argc, char** argv );
	bool Init();
	void Run();
	void Loop();
	void Shutdown();

	void HandleEvents();
	void Update();
	void Render();

	void DisplaySplash( f32 _Duration );

	static long __stdcall WindowProc( LkWindow* _Window, UINT _uMsg, WPARAM _wParam, LPARAM _lParam );

	//////////////////////////////////////////////////////////////////////////
	// To be called at start of frame.
	void _PrepareFrameTime();

	//////////////////////////////////////////////////////////////////////////
	// To be called at end of frame.
	void _CalculateFrameTime();

	void _CapFrameRate();

	// An std::list of parsed command line arguments.
	CommandLineParameters m_CommandLineArguments;

	//HINSTANCE m_hInstance;		// Holds The Instance Of The Application
	bool m_Exit;
	LkWindow* m_Window;

	game::LkGame* m_Game;

	util::Clock m_EngineClock;
	util::Clock m_FrameClock;
	f32 m_FrameTime;	// Seconds.

	//////////////////////////////////////////////////////////////////////////
	// Used to cap the frame rate.
	uint32 m_FrameRateCap;
	f32 m_TargetFrameTime;
};

extern LkEngine* g_Engine;

}

#endif