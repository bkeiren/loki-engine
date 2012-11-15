#include <Windows.h>
#include "core/engine.h"
#include "core/console/console.h"
#include "core/console/consolecommands.h"
#include "core/game/game.h"
#include "core/script/squirrel/squirrel.h"
#include "core/script/lua/lua.h"
#include "core/window.h"
#include "core/renderer/renderer.h"
#include "core/entitysystem/component/default/CameraComponent.h"
#include "core/entitysystem/Entity.h"
#include "core/physics/physics.h"
#include "core/time/Time.h"

namespace loki
{

namespace
{

//////////////////////////////////////////////////////////////////////////
// Scans a string for 'true', 'TRUE', 'false' and 'FALSE'.
// Returns 0 if it matches a false case, 1 if it matches a true case.
// If none of the cases is found, returns 3.
//////////////////////////////////////////////////////////////////////////
int32 ScanStringForBoolean( const std::string& _Str )
{
	if (!_Str.compare("true") || !_Str.compare("TRUE"))
	{
		return (int32)true;
	}
	else if (!_Str.compare("false") || !_Str.compare("FALSE"))
	{
		return (int32)false;
	}
	return 3;
}

bool ScanValueForBoolean( LkConsole::Value _V )
{
	bool b = false;
	int32 i = ScanStringForBoolean(_V.m_String);
	if (i == 0)
	{
		b = false;
	}
	else if (i == 1)
	{
		b = true;
	}
	else
	{
		b = _V.m_Boolean;
	}
	return b;
}

}

CONSOLE_FUNCTION(Console_GetEngineVersion)
{
	LkConsole::CommandResult res("Engine version: v%i.%i.%i ('%s') (%s build)", loki::VersionMajor, loki::VersionMinor, loki::VersionBuild, loki::VersionName, loki::VersionType);
	return res;
}

CONSOLE_FUNCTION(Console_GetGameVersion)
{
	LkConsole::CommandResult res("Game version: '%s' v%i.%i.%i", g_Console->GetEngine()->GetGame()->GameName, 
															   g_Console->GetEngine()->GetGame()->GameVersionMajor, 
															   g_Console->GetEngine()->GetGame()->GameVersionMinor, 
															   g_Console->GetEngine()->GetGame()->GameVersionBuild);
	return res;
}

CONSOLE_FUNCTION(Console_PrintText)
{
	LkConsole::CommandResult res("");
	res.m_Result += "'";
	res.m_Result += _Command->m_Arguments[0].m_String;
	res.m_Result += "'";
	return res;
}

CONSOLE_FUNCTION(Console_Godmode)
{
	LkConsole::CommandResult res("This command should switch godmode ");
	res.m_Result += (ScanValueForBoolean(_Command->m_Arguments[0]))?("on"):("off");
	return res;
}

CONSOLE_FUNCTION(Console_Noclip)
{
	LkConsole::CommandResult res("This command should witch noclip mode ");
	res.m_Result += (ScanValueForBoolean(_Command->m_Arguments[0]))?("on"):("off");
	return res;
}

CONSOLE_FUNCTION(Console_PrintAllCommands)
{
	std::list<std::string> list = g_Console->GetCommandsList();
	LkConsole::CommandResult res("Available commands:");
	for (std::list<std::string>::iterator it = list.begin(); it != list.end(); ++it)
	{
		res.m_Result += "\n\t";
		res.m_Result += (*it);
	}
	return res;
}

CONSOLE_FUNCTION(Console_ReloadConsoleUI)
{
	LkConsole::CommandResult res("Reloading console UI...");
	g_Console->ReloadUI();
	return res;
}

CONSOLE_FUNCTION(Console_GetEngineUpTime)
{
	f32 s = g_Time->GetGlobalTime();	// Seconds.
	f32 m = s / 60.0f;	// Minutes.
	f32 h = s / 3600.0f;	// Hours.

	int32 h2 = (int32)h;
	int32 m2 = (int32)(m - ((int32)h * 60));
	f32 s2 = s - (h2 * 3600) - (m2 * 60);

	LkConsole::CommandResult res("Engine up-time: %02ih:%02im:%02.4fs", h2, m2, s2);
	return res;
}

CONSOLE_FUNCTION(Console_GetFrameRate)
{
	LkConsole::CommandResult res("Framerate: %.3f FPS (Framecap is %s)", g_Engine->GetFrameRate(), (g_Engine->GetFrameCapEnabled())?("enabled"):("disabled"));
	return res;
}

CONSOLE_FUNCTION(Console_GetTimeStamp)
{
	std::string s;
	util::time::GetTimeStamp(s);
	LkConsole::CommandResult res("Timestamp: %s", s.c_str());
	return res;
}

CONSOLE_FUNCTION(Console_CloseConsole)
{
	g_Console->ToggleVisible();
	LkConsole::CommandResult res("Console closed");
	return res;
}

CONSOLE_FUNCTION(Console_BGConsole)
{
	bool b = ScanValueForBoolean(_Command->m_Arguments[0]);
	if (b)
	{
		g_Engine->ShowBackgroundConsoleWindow();
		LkConsole::CommandResult res("Showing background console");
		return res;
	}
	g_Engine->HideBackgroundConsoleWindow();
	LkConsole::CommandResult res("Hiding background console");
	return res;
}

CONSOLE_FUNCTION(Console_CommandDescription)
{
	std::string desc;
	if (g_Console->GetDescriptionOfCommand(_Command->m_Arguments[0].m_String.c_str(), desc))
	{
		return LkConsole::CommandResult(const_cast<char*>(desc.c_str()));
	}
	return LkConsole::CommandResult("'%s' is not a recognized command", _Command->m_Arguments[0].m_String.c_str());
}

CONSOLE_FUNCTION(Console_SquirrelRunScript)
{
	g_Squirrel->RunScript(_Command->m_Arguments[0].m_String.c_str());
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_SquirrelRunScriptAsync)
{
	g_Squirrel->RunScriptAsync(_Command->m_Arguments[0].m_String.c_str());
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_LuaRunScript)
{
	g_Lua->RunScript(_Command->m_Arguments[0].m_String.c_str());
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_LuaRunScriptAsync)
{
	g_Lua->RunScriptAsync(_Command->m_Arguments[0].m_String.c_str());
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_LuaRunString)
{
	g_Lua->RunString(_Command->m_Arguments[0].m_String.c_str());
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_LuaRunStringAsync)
{
	g_Lua->RunStringAsync(_Command->m_Arguments[0].m_String.c_str());
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_CamSetPos)
{
	f32 x = _Command->m_Arguments[0].m_Float;
	f32 y = _Command->m_Arguments[1].m_Float;
	f32 z = _Command->m_Arguments[2].m_Float;
	components::CameraComponent::GetActiveCamera()->GetEntity()->GetTransform().SetPosition(vec3(x, y, z));
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_CamGetPos)
{
	vec3 p = components::CameraComponent::GetActiveCamera()->GetEntity()->GetTransform().GetPosition();
	return LkConsole::CommandResult("CamPos: [%f, %f, %f]", p.x, p.y, p.z);
}

CONSOLE_FUNCTION(Console_CamSetOrientation)
{
	f32 w = _Command->m_Arguments[0].m_Float;
	f32 x = _Command->m_Arguments[1].m_Float;
	f32 y = _Command->m_Arguments[2].m_Float;
	f32 z = _Command->m_Arguments[3].m_Float;
	components::CameraComponent::GetActiveCamera()->GetEntity()->GetTransform().SetOrientation(quat(w, x, y, z));
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_CamGetOrientation)
{
	quat o = components::CameraComponent::GetActiveCamera()->GetEntity()->GetTransform().GetOrientation();
	return LkConsole::CommandResult("CamOrientation: [%f, %f, %f, %f]", o.w, o.x, o.y, o.z);	
}

CONSOLE_FUNCTION(Console_CamSetFoV)
{
	components::CameraComponent::GetActiveCamera()->SetFieldOfView(_Command->m_Arguments[0].m_Float);
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_CamGetFoV)
{
	return LkConsole::CommandResult("CamFoV: %f", components::CameraComponent::GetActiveCamera()->GetFieldOfView());
}

CONSOLE_FUNCTION(Console_CamSetZNear)
{
	components::CameraComponent::GetActiveCamera()->SetNearPlane(_Command->m_Arguments[0].m_Float);
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_CamSetZFar)
{
	components::CameraComponent::GetActiveCamera()->SetFarPlane(_Command->m_Arguments[0].m_Float);
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_CamGetZPlanes)
{
	return LkConsole::CommandResult("Cam ZNear: %f\tZFar: %f", components::CameraComponent::GetActiveCamera()->GetNearPlane(), 
															   components::CameraComponent::GetActiveCamera()->GetFarPlane());
}

CONSOLE_FUNCTION(Console_CamLookAt)
{
	float x = _Command->m_Arguments[0].m_Float;
	float y = _Command->m_Arguments[1].m_Float;
	float z = _Command->m_Arguments[2].m_Float;
	components::CameraComponent::GetActiveCamera()->LookAt(vec3(x, y, z));
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_SetFullscreen)
{
	Window* window = g_Engine->GetWindow();
	bool o = window->IsFullscreen();
	bool b = ScanValueForBoolean(_Command->m_Arguments[0]);
	window->SetFullscreen(b);
	bool f = window->IsFullscreen();
	if (o == b)
	{
		return LkConsole::CommandResult("You are already in %s mode", (o)?("fullscreen"):("windowed"));
	}
	return LkConsole::CommandResult("%s to %s mode", (f == b)?("Switched"):("Failed to switch to"), (b)?("fullscreen"):("windowed"));
}

CONSOLE_FUNCTION(Console_SetWindowPos)
{
	Window* window = g_Engine->GetWindow();
	int32 x = _Command->m_Arguments[0].m_Int;
	int32 y = _Command->m_Arguments[1].m_Int;
	window->SetPosition(x, y);
	return LkConsole::CommandResult("Set window position to [%i, %i]", x, y);
}

CONSOLE_FUNCTION(Console_SetWindowSize)
{
	Window* window = g_Engine->GetWindow();
	int32 x = _Command->m_Arguments[0].m_Int;
	int32 y = _Command->m_Arguments[1].m_Int;
	window->SetDimensions(x, y);
	return LkConsole::CommandResult("Set window size to [%i, %i]", x, y);
}

CONSOLE_FUNCTION(Console_Terminate)
{
	g_Engine->RequestExit();
	return LkConsole::CommandResult("Requested termination...");
}

CONSOLE_FUNCTION(Console_GBufferTargets)
{
#ifdef DBG_VISUALIZATIONS
	renderer::g_Renderer->ToggleVisualizeRenderTargets();
	return LkConsole::CommandResult("");
#else
	return LkConsole::CommandResult("This command is only available in a build with DBG_VISUALIZATIONS defined");
#endif
}

CONSOLE_FUNCTION(Console_LightVolumes)
{
#ifdef DBG_VISUALIZATIONS
	renderer::g_Renderer->ToggleVisualizeLightVolumes();
	return LkConsole::CommandResult("");
#else
	return LkConsole::CommandResult("This command is only available in a build with DBG_VISUALIZATIONS defined");
#endif
}

CONSOLE_FUNCTION(Console_PhysicsDebugDraw)
{
	if (physics::g_Physics->DebugDrawingWasCompiled())
	{
		physics::g_Physics->SetDebugDrawingEnabled(_Command->m_Arguments[0].m_Boolean);
		return LkConsole::CommandResult("");
	}
	return LkConsole::CommandResult("Physics debug drawing was not compiled into this build (PHY_DEBUG_DRAW was not defined at compile-time)");
}

CONSOLE_FUNCTION(Console_PhysicsSetGravity)
{
	physics::g_Physics->SetGravity(vec3(_Command->m_Arguments[0].m_Float, _Command->m_Arguments[1].m_Float, _Command->m_Arguments[2].m_Float));
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_TimeSetScale)
{
	g_Time->SetTimeScale(_Command->m_Arguments[0].m_Float);
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_TimeGetScale)
{
	return LkConsole::CommandResult("Time scale: %f", g_Time->GetTimeScale());
}

}	// Namespace loki.
