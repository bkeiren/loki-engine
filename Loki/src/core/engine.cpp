//#include "core/commandlineparsing.h"

#include "core/states/Splashstate.h"
#include "core/states/Teststate.h"
#include "core/renderer/mrtobject.h"	// TODO: Remove.
#include "core/renderer/renderer.h"
#include "core/engine.h"
#include "core/window.h"

#include "core/renderer/scene/scene.h"		// TODO: Remove.
#include "core/actor/camera/camera.h"	// TODO: Remove.

#include "util/util.h"

#include "core/actor/actor.h"

#include "core/console/console.h"

//#include "math/glm/gtx/rotate_vector.hpp"

#include "util/thread/thread.h"
#include "util/jobmanager/jobmanager.h"

#include "core/script/lua/lua.h"
#include "core/script/squirrel/squirrel.h"

#include "core/physics/physics.h"

#include "core/audio/audio.h"

#include "util/systeminfo/systeminfo.h"

#include "core/html/htmlcore.h"

#include <time.h>

#include "core/game/game.h"

#include "core/game/localization/localization.h"

#include "core/ui/overlaymanager.h"

#include "core/renderer/image/image.h"
#include "core/renderer/image/animatedimage.h"

#ifdef _DEBUG
// Debug defines.
	#define WINDOW_WIDTH			1280
	#define WINDOW_HEIGHT			720
	#define WINDOW_FULLSCREEN		false
	#define WINDOW_CAPTION			"Loki (Debug)"
	#define WINDOW_BITDEPTH			32
	#define WINDOW_X				0
	#define WINDOW_Y				0
#else
// Release defines.
	#define WINDOW_WIDTH			1280
	#define WINDOW_HEIGHT			720
	#define WINDOW_FULLSCREEN		false
	#define WINDOW_CAPTION			"Loki"
	#define WINDOW_BITDEPTH			32
	#define WINDOW_X				0
	#define WINDOW_Y				0
#endif

namespace loki
{
//////////////////////////////////////////////////////////////////////////
// Data.
//////////////////////////////////////////////////////////////////////////

const int VersionMajor = 0;
const int VersionMinor = 0;
const int VersionBuild = 1;
const char* VersionName = "Loki";
#ifdef _DEBUG
const char* VersionType = "Debug";
#else
const char* VersionType = "Release";
#endif

LkEngine* g_Engine = NULL;

loki::LkHTMLView* webtab = NULL;	// REMOVE.

//////////////////////////////////////////////////////////////////////////
// C-tor.
//////////////////////////////////////////////////////////////////////////
LkEngine::LkEngine( game::LkGame* _Game )	:
	m_Exit(false),
	m_Game(_Game),
	m_FrameTime(1.0f),
	m_FrameRateCap(0),
	m_TargetFrameTime(0.0f),
	m_Window(0)
{
#ifndef _DEBUG
	HideBackgroundConsoleWindow();
#endif

	assert(m_Game != NULL);

	if (g_Engine != NULL)
	{
		LOG(VL_ERROR, "Engine::Engine: Global g_Engine variable already has a value");
		exit(0);
		// Do we want to allow multiple instances of Engine object? (This does not mean application, I'm talking about Engine instances)
	}
	g_Engine = this;
}

LkEngine::LkEngine()
{
	ILLEGAL_CTOR_ERROR("Engine");
}

//////////////////////////////////////////////////////////////////////////
// D-tor.
//////////////////////////////////////////////////////////////////////////
LkEngine::~LkEngine()
{
	
}

void LkEngine::Go( int argc, char** argv )
{
	m_EngineClock.Start();

	// Init console and logger (In this order!).
	// These are initalized in Go() instead of Init() because they need to be initalized as one of the first components.
	g_Console = new LkConsole(this);
	g_Logger = new LkLogger();

	// Log start-up message.
	LOG(VL_ALWAYS, "Loki v%i.%i.%i started.", loki::VersionMajor, loki::VersionMinor, loki::VersionBuild);

	// Splash window.
#ifndef _DEBUG
	//DisplaySplash(2000);
#endif

	// Parse command line arguments.
	loki::LkEngine::ParseArguments(argc, argv);
	if (loki::LkEngine::Init())	// Call init.
	{
		loki::LkEngine::Run();	// Run the engine.
	}
	else
	{
		LOG(VL_ERROR, "Engine::Init: Failed");
	}
	loki::LkEngine::Shutdown();	// Shutdown.
}

LkWindow* LkEngine::GetWindow() const
{
	return m_Window;
}

game::LkGame* LkEngine::GetGame()
{
	return m_Game;
}

float LkEngine::GetFrameTime() const
{
	return m_FrameTime;
}

float LkEngine::GetEngineUpTime() const
{
	return m_EngineClock.Lap();
}

float LkEngine::GetFrameRate() const
{
	return (1.0f / m_FrameTime);
}

void LkEngine::SetFrameRateCap( unsigned int _Cap )
{
	m_FrameRateCap = _Cap;
	m_TargetFrameTime = 1.0f / m_FrameRateCap;
}

bool LkEngine::GetFrameCapEnabled() const
{
	return (m_FrameRateCap <= 0);
}

void LkEngine::ShowBackgroundConsoleWindow() const
{
	ShowWindow(GetConsoleWindow(), SW_RESTORE);
}

void LkEngine::HideBackgroundConsoleWindow() const
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
// Processes the command line arguments.
// This function should be called first. If it is not called, that's not a
// problem, however any command line arguments can not be used
// while initializing the engine.
//////////////////////////////////////////////////////////////////////////
void LkEngine::ParseArguments( int argc, char** argv )
{
	LOG(VL_ALWAYS, "Parsing command line...");

	ParseCommandLine(argc, argv, m_CommandLineArguments);
}

//////////////////////////////////////////////////////////////////////////
// Initializes the render window and other things required to run
// the engine. This function should be called after Engine::ProcessArguments.
//////////////////////////////////////////////////////////////////////////
bool LkEngine::Init()
{
	srand((unsigned int)time(0));

	// Collect system information.
	util::g_SystemInfo = new util::LkSystemInfo();
#ifdef _DEBUG
	util::g_SystemInfo->LogSystemInformation();
#endif

	// Initialize localization object.
	game::g_Localization = new game::LkLocalization();

	// Initialize job manager.
	util::g_JobManager = new util::LkJobManager(util::g_SystemInfo->GetNumProcessors(), 128);

	// Initialize event manager.
	g_EventManager = new LkEventManager();

	// Initialize physics.
	physics::g_Physics = new physics::LkPhysics();

	// Initialize audio.
	g_Audio = new LkAudio();

	// Initialize Lua state.
	g_Lua = new LkLua();

	// Initialize Squirrel state.
	g_Squirrel = new LkSquirrel();

	// Initialize input.
	g_Input = new LkInput();

	// Initialize overlay manager.
	ui::g_OverlayManager = new ui::LkOverlayManager();

	// Lua examples.
	if (false)
	{
		g_Lua->RunScript("resources//scripts//script1.lua");
		g_Lua->RunString("c = 7 LOG(c)");
		g_Lua->RunString("local b = 6");
		g_Lua->RunString("LOG(b)");

		g_Lua->RunStringAsync("LOG(\"This is asynchronous\")");
		g_Lua->RunScriptAsync("resources//scripts//script1.lua");
		g_Lua->RunScriptAsync("resources//scripts//script1.lua");
		g_Lua->RunStringAsync("LOG(\"This is asynchronous\")");
	}

	// TODO: Implement config loader

	// Initialize the renderer.
	m_Window = new LkWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CAPTION, WINDOW_BITDEPTH, WINDOW_FULLSCREEN, LkEngine::WindowProc, WINDOW_X, WINDOW_Y);
	m_Window->MakeRenderContextCurrent();
	/*if (!LkEngine::CreateGLWindow(WINDOW_CAPTION, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BITDEPTH, WINDOW_FULLSCREEN))*/
	if (!m_Window->IsValid())
	{
		LOG(VL_ERROR, "Unable to create render window (Settings: %i x %i @ %i [fullscr: %i]).", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BITDEPTH, WINDOW_FULLSCREEN);
		return false;
	}
	renderer::g_Renderer = new renderer::LkRenderer(m_Window);
	
	// Initialize web browser.
	g_HTMLCore = new LkHTMLCore();

	// Finalize console UI.
	g_Console->FinalizeInitialization();

	// Webbrowser tab creation and page loading + rendering.
	if (true)
	{
		webtab = g_HTMLCore->CreateView(renderer::g_Renderer->GetRenderWidth(), renderer::g_Renderer->GetRenderHeight());
		//webtab->LoadURL("http://www.google.com/");
		//webtab->LoadFile("resources//ui//test1//page.html");
		webtab->LoadFile("resources//ui//index.html");
		webtab->CreateJavascriptObject(L"TestObject");
		webtab->SetJavascriptCallback(L"TestObject", L"MyCallback");
		webtab->ExecuteJavascript("TestObject.MyCallback();");
		webtab->ExecuteJavascript("TestObject.MyCallback(\"Je Oma Is Lelijk :D\");");
		webtab->ExecuteJavascript("TestObject.MyCallback(0, 1, 2, 3, 4);");
	}

	// Just for testing.
	//g_Console->Execute("godmode 1");
	//g_Console->Execute("getEngineVersion");
	//g_Console->Execute("getGameVersion");

	// Register states.
	g_StateManager = new LkStateManager();
	g_StateManager->RegisterState(new State_Splash("State_Splash"));
	g_StateManager->RegisterState(new State_Test("State_Test"));

	// Set the state to start up with.
	//StateManager::SetActiveState("State_Splash");

	m_Game->PreInit();
	if (!m_Game->Init())
	{
		LOG(VL_ERROR, "Engine::Init: Game failed to initialize");
		m_Game->PostInitFail();
		return false;
	}
	m_Game->PostInit();

	LOG(VL_ALWAYS, "Game::Init: Initialized");
	
	//Sound* sound = Audio::CreateSound("resources//sound//song.mp3");
	//sound->Play();

	// Squirrel base script that contains base functionality and data that all squirrel scripts need to be able
	// to access.
// 	Squirrel::RunScript("resources//scripts//squirrel_base.nut");
// 
// 	Squirrel::RunScript("resources//scripts//squirrelscript1.nut");
// 	Squirrel::RunScript("resources//scripts//squirrelscript2.nut");

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Starts the main application loop.
// This is the function that should be called after Engine::Init.
//////////////////////////////////////////////////////////////////////////
void LkEngine::Run()
{
	Loop();

	/*
	// Even though the application should not have been gotten out of the main application loop if the application window is still opened, it's 
	// good to double check and close it here if it is still open.
	if (g_RenderWindow.IsOpened())
	{
		LOG(VL_WARN, "Exited main loop before window was closed, there may be an error in the code.");
		g_RenderWindow.Close();
	}
	*/
}

//////////////////////////////////////////////////////////////////////////
// The main application loop. Is called by Engine::Run.
//////////////////////////////////////////////////////////////////////////
void LkEngine::Loop()
{
	while (/*g_RenderWindow.IsOpened() ||*/ !m_Exit)
	{
		_PrepareFrameTime();

		HandleEvents();

		g_EventManager->Post(LkEvent(EVENT_FRAMESTART));
		g_EventManager->Post(LkEvent(EVENT_PREUPDATE));

		// Update the application logic.
		Update();

		g_EventManager->Post(LkEvent(EVENT_POSTUPDATE));
		g_EventManager->Post(LkEvent(EVENT_PRERENDER));

		// Render.
#ifndef _DEBUG
		physics::g_Physics->DebugDraw();
#endif
		Render();

		g_EventManager->Post(LkEvent(EVENT_FRAMEEND));

		_CalculateFrameTime();
		_CapFrameRate();
	}
}

//////////////////////////////////////////////////////////////////////////
// Cleans up certain things.
//////////////////////////////////////////////////////////////////////////
void LkEngine::Shutdown()
{
	m_Game->PreShutdown();
	m_Game->Shutdown();
	m_Game->PostShutdown();

	LOG(VL_ALWAYS, "Game::Shutdown: Done");

	delete m_Game;
	m_Game = NULL;

	LOG(VL_ALWAYS, "Loki terminating...");

	delete g_StateManager;
	g_StateManager = NULL;

	// Same kind of story as with LkConsole::FinalizeInitialization.
	g_Console->Deinitialize();

	g_HTMLCore->DestroyView(&webtab);
	delete g_HTMLCore;
	g_HTMLCore = NULL;

	delete m_Window;
	m_Window = NULL;
	
	delete renderer::g_Renderer;
	renderer::g_Renderer = NULL;

	// Close overlay manager.
	delete ui::g_OverlayManager;
	ui::g_OverlayManager = NULL;

	// Close input.
	delete g_Input;
	g_Input = NULL;

	// Close Squirrel.
	delete g_Squirrel;
	g_Squirrel = NULL;

	// Close Lua state.
	delete g_Lua;
	g_Lua = NULL;

	// Close audio.
	delete g_Audio;
	g_Audio = NULL;

	// Close physics.
	delete physics::g_Physics;
	physics::g_Physics = NULL;

	// Close event manager.
	delete g_EventManager;
	g_EventManager = NULL;

	// Delete job manager.
	delete util::g_JobManager;
	util::g_JobManager = NULL;

	// Delete localization object.
	delete game::g_Localization;
	game::g_Localization = NULL;

	LOG(VL_ALWAYS, "-- Goodbye...");

	// Close the logger and dump the final logging data.
	delete g_Logger;
	g_Logger = NULL;

	delete g_Console;
	g_Console = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Handles window input events.
//////////////////////////////////////////////////////////////////////////
void LkEngine::HandleEvents()
{
	/*
	case sf::Key::F12:
		{
			sf::Image ScreenShot = g_RenderWindow.Capture();
			time_t rawtime;
			tm* LocalTime = new tm();
			time(&rawtime);
			localtime_s(LocalTime, &rawtime);

			static char filepath[128];
			sprintf_s(filepath, "screenshots//screenshot_%i%i%i%i%i%i.jpg",	LocalTime->tm_mday,
																			LocalTime->tm_mon + 1,		// + 1 because the month is counted from 0 to 11, 
																										// so February would be 1 (while we want to have 2).
																			LocalTime->tm_year - 100,	// - 100 because the year is counted from 1900, so 2011 would be 111.
																			LocalTime->tm_hour,
																			LocalTime->tm_min,
																			LocalTime->tm_sec,
																			".jpg");
			if (ScreenShot.SaveToFile(filepath))
			{
				LOG(VL_ALWAYS, "Screenshot saved (%s)", filepath);
			}
			break;
		}
	*/

	// Capture input from the OS.
	g_Input->Capture();

	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
	{
		switch (msg.message)
		{
		case WM_QUIT:
			{
				m_Exit = true;
				break;
			}
		case WM_MOUSEWHEEL:
			{
				g_Input->m_MouseWheelDelta = (float)(GET_WHEEL_DELTA_WPARAM(msg.wParam) / 120);
				g_EventManager->Post(LkEvent(EVENT_MOUSEWHEELMOVE));
				break;
			}
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_CHAR:
		//case WM_IMECHAR:
		case WM_SYSCHAR:
			{
				g_HTMLCore->InjectKeyboardEvent(msg.message, msg.wParam, msg.lParam);
				break;
			}
		default:
			{
				break;
			}
		}
		TranslateMessage(&msg);				// Translate The Message
		DispatchMessage(&msg);				// Dispatch The Message
	}
}

//////////////////////////////////////////////////////////////////////////
// The main update function. 
// Updates logic, but does NOT perform render calls.
//////////////////////////////////////////////////////////////////////////
void LkEngine::Update()
{
	m_Window->UpdateCursorImage();

	// Update audio.
	g_Audio->Update();

	// Update the currently active state.
	//StateManager::GetActiveState()->Update();

	m_Game->PreUpdate();

	g_EventManager->Post(EVENT_ONUPDATE);

	m_Game->Update();

	g_EventManager->Post(EVENT_PREPHYSICSUPDATE);
	physics::g_Physics->Update();
	g_EventManager->Post(EVENT_POSTPHYSICSUPDATE);

	m_Game->PostUpdate();

// 	if (g_Input->Get(KEY_R) == KEYSTATE_RELEASED)
// 	{
// 		renderer::g_Renderer->ToggleVisualizeRenderTargets();
// 	}

	if (g_Input->Get(KEY_ESCAPE) == KEYSTATE_RELEASED)
	{
		//if (util::MessageBoxConfirmation("Are you sure you want to exit?", "Exit", true) == CONFIRMATION_YES)
		//{
			m_Exit = true;
		//}
	}
}

//////////////////////////////////////////////////////////////////////////
// The main render function.
//////////////////////////////////////////////////////////////////////////
void LkEngine::Render()
{
	// Render the currently active state.
	//StateManager::GetActiveState()->Render();

	renderer::g_Renderer->Render( m_Game->m_Level );

	webtab->Render();
// 	if (g_HTMLCore->GetWebTabInFocus() == 0)
// 	{
// 		g_HTMLCore->SetFocus(webtab);
// 	}

	static renderer::LkImage* img = new renderer::LkImage("resources//textures//default.bmp", glm::vec2(0.0f, 0.0f), glm::vec2(0.1f, 0.1f));
	glm::int2 m = g_Input->GetMousePosition();
	img->SetAbsolutePosition(glm::vec2(m.x, m.y));
	img->Render();

	g_Console->Render();

	g_EventManager->Post(LkEvent(EVENT_POSTRENDER));

	m_Window->SwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// Displays the splash screen for _Duration (In milliseconds).
//////////////////////////////////////////////////////////////////////////
void LkEngine::DisplaySplash( float _Duration )
{
	
}

//////////////////////////////////////////////////////////////////////////
// The Window callback function to handle window messages.
//////////////////////////////////////////////////////////////////////////
long __stdcall LkEngine::WindowProc( LkWindow* _Window, UINT _uMsg, WPARAM _wParam, LPARAM _lParam )
{
	switch (_uMsg)									// Check For Windows Messages
	{
	case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (_wParam)							// Check System Calls
			{
			case SC_SCREENSAVE:					// Screensaver Trying To Start?
			case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}
	case WM_CLOSE:								// Did We Receive A Close Message?
		{
			//PostQuitMessage(0);						// Send A Quit Message
			g_Engine->RequestExit();
			return 0;								// Jump Back
		}
	case WM_SIZE:
		{
			g_EventManager->Post(LkEvent(EVENT_WINDOWRESIZE));
			return 0;
		}
	}

	return -1;
}

void LkEngine::RequestExit()
{
	m_Exit = true;
}

void LkEngine::_PrepareFrameTime()
{
	m_FrameClock.Start();
}

void LkEngine::_CalculateFrameTime()
{
	m_FrameTime = m_FrameClock.Lap();
}

void LkEngine::_CapFrameRate()
{
	// Cap the frame rate if a cap is enabled by filling spare time with a call to Sleep().
	if ((bool)m_FrameRateCap && m_FrameTime < m_TargetFrameTime)
	{
		util::Sleep((unsigned int)((m_TargetFrameTime - m_FrameTime) * 1000));
		m_FrameTime = m_TargetFrameTime;
	}
}

}