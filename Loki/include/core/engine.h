#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#ifndef _DEBUG				// If release build...
	#if _SECURE_SCL != 0	// If STL containers and iterators use run-time checks...
		// Yield an error because the library is build without run-time checks for release builds, but the current project
		// that's including this file has not disabled these checks.
		#error	The Loki lib file was compiled without STL run-time checks, yet your project has them enabled. This will cause errors.	\
				Disable them by setting _SECURE_SCL to 0 in your preprocessor defines.
	#endif
	#if _HAS_ITERATOR_DEBUGGING != 0
		#error	The Loki lib file was compiled without STL iterator run-time checks, yet your project has them enabled. This will cause errors.	\
				Disable them by setting _HAS_ITERATOR_DEBUGGING to 0 in your preprocessor defines.
	#endif
#endif

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

class Window;

//////////////////////////////////////////////////////////////////////////
// Static functions
//////////////////////////////////////////////////////////////////////////
class LokiEngine
{
public:
	LokiEngine( game::LkGame* _Game );
	~LokiEngine();

	void Go( int32 argc, char** argv );

	Window* GetWindow() const;
	game::LkGame* GetGame();

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

	bool IsInEditorMode() const;
private:
	LokiEngine();

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

	static long __stdcall WindowProc( Window* _Window, UINT _uMsg, WPARAM _wParam, LPARAM _lParam );

	void _CapFrameRate();

	void _BuildEditorMenus();

	// A map of parsed command line arguments.
	CommandLineParameters m_CommandLineArguments;

	//HINSTANCE m_hInstance;		// Holds The Instance Of The Application
	bool m_Exit;
	Window* m_Window;

	game::LkGame* m_Game;

	//////////////////////////////////////////////////////////////////////////
	// Used to cap the frame rate.
	uint32 m_FrameRateCap;
	f32 m_TargetFrameTime;

	bool m_EditorMode;
};

extern LokiEngine* g_Engine;

}

#endif