//#include "core/commandlineparsing.h"

#include "core/states/Splashstate.h"
#include "core/states/Teststate.h"
#include "core/renderer/renderer.h"
#include "core/engine.h"
#include "core/window/Window.h"

#include "util/util.h"

#include "core/console/console.h"

#include "util/thread/thread.h"
#include "util/jobmanager/jobmanager.h"

#include "core/script/lua/lua.h"
#include "core/script/squirrel/squirrel.h"

#include "core/physics/physics.h"

#include "core/audio/audio.h"

#include "util/systeminfo/systeminfo.h"

#include "core/html/htmlcore.h"
#include "core/html/nullhtmlcore.h"

#include <time.h>

#include "core/game/game.h"

#include "core/game/localization/localization.h"

#include "core/ui/overlaymanager.h"

#include "core/renderer/image/image.h"
#include "core/renderer/image/animatedimage.h"

#include "core/entitysystem/EntitySystem.h"

#include "core/input/input.h"

#include "core/time/Time.h"

#include "util/dragdrophandler/DragDropHandler.h"

#include "core/gui/GUI.h"

#include "core/window/Win32SubMenu.h"
#include "core/window/Win32MenuItem.h"

#include "core/editor/MenuCallbacks.h"

#ifdef _DEBUG
// Debug defines.
	#define WINDOW_WIDTH			1280
	#define WINDOW_HEIGHT			720
	#define WINDOW_FULLSCREEN		false
	#define WINDOW_CAPTION			"Loki (Debug)"
	#define WINDOW_BITDEPTH			32
	#define WINDOW_X				0
	#define WINDOW_Y				0
	#define EDITOR_CAPTION			"LokiEd (Debug)"
#else
// Release defines.
	#define WINDOW_WIDTH			1280
	#define WINDOW_HEIGHT			720
	#define WINDOW_FULLSCREEN		false
	#define WINDOW_CAPTION			"Loki"
	#define WINDOW_BITDEPTH			32
	#define WINDOW_X				0
	#define WINDOW_Y				0
	#define EDITOR_CAPTION			"LokiEd"
#endif

namespace loki
{
//////////////////////////////////////////////////////////////////////////
// Data.
//////////////////////////////////////////////////////////////////////////

const int32 VersionMajor = 0;
const int32 VersionMinor = 0;
const int32 VersionBuild = 1;
const char* VersionName = "Loki";
#ifdef _DEBUG
const char* VersionType = "Debug";
#else
const char* VersionType = "Release";
#endif

LokiEngine* g_Engine = NULL;

loki::LkHTMLView* webtab = NULL;	// REMOVE.

//////////////////////////////////////////////////////////////////////////
// C-tor.
//////////////////////////////////////////////////////////////////////////
LokiEngine::LokiEngine( game::LkGame* _Game )	:
	m_Exit(false),
	m_Game(_Game),
	m_FrameRateCap(0),
	m_TargetFrameTime(0.0f),
	m_Window(0),
	m_EditorMode(false)
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

LokiEngine::LokiEngine()
{
	ILLEGAL_CTOR_ERROR("Engine");
}

//////////////////////////////////////////////////////////////////////////
// D-tor.
//////////////////////////////////////////////////////////////////////////
LokiEngine::~LokiEngine()
{
	
}

void LokiEngine::Go( int32 argc, char** argv )
{
	g_Time = new Time();

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
	loki::LokiEngine::ParseArguments(argc, argv);
	if (loki::LokiEngine::Init())	// Call init.
	{
		loki::LokiEngine::Run();	// Run the engine.
	}
	else
	{
		LOG(VL_ERROR, "Engine::Init: Failed");
	}
	loki::LokiEngine::Shutdown();	// Shutdown.
}

Window* LokiEngine::GetWindow() const
{
	return m_Window;
}

game::LkGame* LokiEngine::GetGame()
{
	return m_Game;
}

f32 LokiEngine::GetFrameRate() const
{
	return (1.0f / g_Time->GetFrameTime());
}

void LokiEngine::SetFrameRateCap( uint32 _Cap )
{
	m_FrameRateCap = _Cap;
	m_TargetFrameTime = 1.0f / m_FrameRateCap;
}

bool LokiEngine::GetFrameCapEnabled() const
{
	return (m_FrameRateCap <= 0);
}

void LokiEngine::ShowBackgroundConsoleWindow() const
{
	ShowWindow(GetConsoleWindow(), SW_RESTORE);
}

void LokiEngine::HideBackgroundConsoleWindow() const
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
// Processes the command line arguments.
// This function should be called first. If it is not called, that's not a
// problem, however any command line arguments can not be used
// while initializing the engine.
//////////////////////////////////////////////////////////////////////////
void LokiEngine::ParseArguments( int32 argc, char** argv )
{
	LOG(VL_ALWAYS, "Parsing command line...");

	ParseCommandLine(argc, argv, m_CommandLineArguments);

	CommandLineParametersConstIter it = m_CommandLineArguments.find("editor");
	if (it != m_CommandLineArguments.end())
	{
		m_EditorMode = true;
		LOG(VL_ALWAYS, "LokiEngine: -editor command line argument detected. Running in editor mode");
	}
}

//////////////////////////////////////////////////////////////////////////
// Initializes the render window and other things required to run
// the engine. This function should be called after Engine::ProcessArguments.
//////////////////////////////////////////////////////////////////////////
bool LokiEngine::Init()
{
	srand((uint32)::time(0));

	// Collect system information.
	util::system::g_SystemInfo = new util::system::SystemInfo();
#ifdef _DEBUG
	util::system::g_SystemInfo->LogSystemInformation();
#endif

	// Initialize localization object.
	game::g_Localization = new game::LkLocalization();

	// Initialize job manager.
	util::general::g_JobManager = new util::general::JobManager(util::system::g_SystemInfo->GetNumProcessors(), 128);

	// Initialize event manager.
	g_EventManager = new LkEventManager();

	// Initialize physics.
	physics::g_Physics = new physics::LkPhysics();

	// Initialize audio.
	audio::g_Audio = new audio::Audio();

	// Initialize Lua state.
	g_Lua = new LkLua();

	// Initialize Squirrel state.
	g_Squirrel = new Squirrel();

	// Initialize input.
	g_Input = new LkInput();

	// Initialize overlay manager.
	ui::g_OverlayManager = new ui::LkOverlayManager();
	
	// TODO: Implement config loader

	// Initialize the renderer.
	if (IsInEditorMode())
	{
		m_Window = new Window(util::system::g_SystemInfo->GetDesktopResolution().x, 
							  util::system::g_SystemInfo->GetDesktopResolution().y, 
							  EDITOR_CAPTION, WINDOW_BITDEPTH, false, LokiEngine::WindowProc, 0, 0);
		m_Window->Maximize();
		m_Window->SetAcceptDragDropFiles(true);
		_BuildEditorMenus();
	}
	else
	{
		m_Window = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CAPTION, WINDOW_BITDEPTH, WINDOW_FULLSCREEN, LokiEngine::WindowProc, WINDOW_X, WINDOW_Y);
	}
	
	m_Window->MakeRenderContextCurrent();
	/*if (!LkEngine::CreateGLWindow(WINDOW_CAPTION, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BITDEPTH, WINDOW_FULLSCREEN))*/
	if (!m_Window->IsValid())
	{
		LOG(VL_ERROR, "Unable to create render window (Settings: %i x %i @ %i [fullscr: %i]).", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BITDEPTH, WINDOW_FULLSCREEN);
		return false;
	}
	renderer::g_Renderer = new renderer::LkRenderer(m_Window);

	g_HTMLCore = IsInEditorMode() ? new NullHTMLCore() : new LkHTMLCore();

	gui::g_GUI = new gui::GUI();

	// Finalize console UI.
	g_Console->FinalizeInitialization();

	// Create entity system.
	g_EntitySystem = CreateEntitySystem();

	// Webbrowser tab creation and page loading + rendering.
	if (false)
	{
		webtab = g_HTMLCore->CreateView(renderer::g_Renderer->GetRenderWidth(), renderer::g_Renderer->GetRenderHeight());
		//webtab->LoadURL("http://www.google.com/");
		//webtab->LoadFile("resources//ui//test1//page.html");
		webtab->LoadFile(DEFAULT_RESOURCE("ui//index.html"));
		webtab->CreateJavascriptObject(L"TestObject");
		webtab->SetJavascriptCallback(L"TestObject", L"MyCallback");
		webtab->ExecuteJavascript("TestObject.MyCallback();");
		webtab->ExecuteJavascript("TestObject.MyCallback(\"Je Oma Is Lelijk :D\");");
		webtab->ExecuteJavascript("TestObject.MyCallback(0, 1, 2, 3, 4);");
	}

	// Register states.
	g_StateManager = new LkStateManager();
	g_StateManager->RegisterState(new State_Splash("State_Splash"));
	g_StateManager->RegisterState(new State_Test("State_Test"));

	// Set the state to start up with.
	//StateManager::SetActiveState("State_Splash");

	
	if (!IsInEditorMode())
	{
		m_Game->PreInit();
		if (!m_Game->Init())
		{
			LOG(VL_ERROR, "Engine::Init: Game failed to initialize");
			m_Game->PostInitFail();
			return false;
		}
		m_Game->PostInit();
		LOG(VL_ALWAYS, "Game::Init: Initialized");
	}
	
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
void LokiEngine::Run()
{
	Loop();
}

//////////////////////////////////////////////////////////////////////////
// The main application loop. Is called by Engine::Run.
//////////////////////////////////////////////////////////////////////////
void LokiEngine::Loop()
{
	while (/*g_RenderWindow.IsOpened() ||*/ !m_Exit)
	{
		g_Time->_PrepareFrameTime();

		HandleEvents();

		g_EventManager->Post(LkEvent(EVENT_FRAMESTART));
		g_EventManager->Post(LkEvent(EVENT_PREUPDATE));

		// Update the application logic.
		Update();

		g_EventManager->Post(LkEvent(EVENT_POSTUPDATE));
		g_EventManager->Post(LkEvent(EVENT_PRERENDER));

		// Render.
		Render();

		g_EventManager->Post(LkEvent(EVENT_FRAMEEND));

		g_Time->_CalculateFrameTime();
		_CapFrameRate();
	}
}

//////////////////////////////////////////////////////////////////////////
// Cleans up certain things.
//////////////////////////////////////////////////////////////////////////
void LokiEngine::Shutdown()
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

	delete g_EntitySystem;
	g_EntitySystem = NULL;

	// Same kind of story as with LkConsole::FinalizeInitialization.
	g_Console->Deinitialize();

	delete gui::g_GUI;
	gui::g_GUI = NULL;

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
	delete audio::g_Audio;
	audio::g_Audio = NULL;

	// Close physics.
	delete physics::g_Physics;
	physics::g_Physics = NULL;

	// Close event manager.
	delete g_EventManager;
	g_EventManager = NULL;

	// Delete job manager.
	delete util::general::g_JobManager;
	util::general::g_JobManager = NULL;

	// Delete localization object.
	delete game::g_Localization;
	game::g_Localization = NULL;

	LOG(VL_ALWAYS, "-- Goodbye...");

	// Close the logger and dump the final logging data.
	delete g_Logger;
	g_Logger = NULL;

	delete g_Console;
	g_Console = NULL;

	delete g_Time;
	g_Time = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Handles window input events.
//////////////////////////////////////////////////////////////////////////
void LokiEngine::HandleEvents()
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
	g_Input->_Capture();

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
				g_Input->m_MouseWheelDelta = (f32)(GET_WHEEL_DELTA_WPARAM(msg.wParam) / 120);
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
		case WM_COMMAND:
			{
				Win32MenuItem::_CallCallback((uint32)LOWORD(msg.wParam));
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
void LokiEngine::Update()
{
	gui::g_GUI->_UpdateContexts();

	m_Window->UpdateCursorImage();

	// Update audio.
	audio::g_Audio->_Update();

	// Update the currently active state.
	//StateManager::GetActiveState()->Update();

	m_Game->PreUpdate();

	g_EventManager->Post(EVENT_ONUPDATE);

	m_Game->Update();

	g_EventManager->Post(EVENT_PREPHYSICSUPDATE);
	physics::g_Physics->_Update();
	g_EventManager->Post(EVENT_POSTPHYSICSUPDATE);

	m_Game->PostUpdate();
}

//////////////////////////////////////////////////////////////////////////
// The main render function.
//////////////////////////////////////////////////////////////////////////
void LokiEngine::Render()
{
	// Do debug drawing if required.
	physics::g_Physics->_DebugDraw();

	// Render the currently active state.
	//StateManager::GetActiveState()->Render();

	renderer::g_Renderer->Render();

	//webtab->Render();
// 	if (g_HTMLCore->GetWebTabInFocus() == 0)
// 	{
// 		g_HTMLCore->SetFocus(webtab);
// 	}

	gui::g_GUI->_RenderContexts();

	static renderer::LkImage* img = new renderer::LkImage(DEFAULT_RESOURCE("textures//default.bmp"), vec2(0.0f, 0.0f), vec2(0.1f, 0.1f));
	int2 m = g_Input->GetMousePosition();
	img->SetAbsolutePosition(vec2(m.x, m.y));
	img->Render();

	g_Console->Render();

	g_EventManager->Post(LkEvent(EVENT_POSTRENDER));

	m_Window->SwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// Displays the splash screen for _Duration (In milliseconds).
//////////////////////////////////////////////////////////////////////////
void LokiEngine::DisplaySplash( f32 _Duration )
{
	
}

//////////////////////////////////////////////////////////////////////////
// The Window callback function to handle window messages.
//////////////////////////////////////////////////////////////////////////
long __stdcall LokiEngine::WindowProc( Window* _Window, UINT _uMsg, WPARAM _wParam, LPARAM _lParam )
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
	case WM_DROPFILES:
		{
			char FileName[MAX_PATH] = "";
			uint32 NumFiles = DragQueryFileA((HDROP)_wParam, 0xffffffff, FileName, MAX_PATH);

			for (uint32 i = 0; i < NumFiles; ++i)
			{
				POINT CursorPosition;
				DragQueryFileA((HDROP)_wParam, i, FileName, MAX_PATH);
				DragQueryPoint((HDROP)_wParam, &CursorPosition);

				util::system::DragDropHandler::_PushDroppedFile(std::string(FileName), int2(CursorPosition.x, CursorPosition.y));
			}
			DragFinish((HDROP)_wParam);
			return 0;
		}
	}

	return -1;
}

void LokiEngine::RequestExit()
{
	m_Exit = true;
}

bool LokiEngine::IsInEditorMode() const
{
	return m_EditorMode;
}

void LokiEngine::_CapFrameRate()
{
	return;
	// Cap the frame rate if a cap is enabled by filling spare time with a call to Sleep().
	if ((bool)m_FrameRateCap && g_Time->GetFrameTime() < m_TargetFrameTime)
	{
		util::system::Sleep((uint32)((m_TargetFrameTime - g_Time->GetActualFrameTime()) * 1000));
		g_Time->_SetFrameTime(m_TargetFrameTime);
	}
}

void LokiEngine::_BuildEditorMenus()
{
	Win32SubMenu* FileSubMenu = m_Window->GetWin32Menu()->CreateSubMenu("&File");
		Win32SubMenu* NewSubMenu = FileSubMenu->CreateSubMenu("&New...");
			NewSubMenu->CreateItem("&Scene", 0)->SetCallback(&NewSceneCallback);
			NewSubMenu->CreateItem("&Material", 0)->SetCallback(&NewMaterialCallback);
			NewSubMenu->CreateItem("&Model", 0)->SetCallback(&NewModelCallback);

	Win32SubMenu* EntitySubMenu = m_Window->GetWin32Menu()->CreateSubMenu("&Entity");
		EntitySubMenu->CreateItem("Create Empty", 0)->SetCallback(&EmptyEntityCallback);
		EntitySubMenu->CreateItem("", Win32SubMenu::ITEM_FLAG_SEPARATOR);
		EntitySubMenu->CreateItem("Find...", 0);

	m_Window->ReloadWin32Menu();
}

}