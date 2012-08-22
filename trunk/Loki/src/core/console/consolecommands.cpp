#include <Windows.h>
#include "core/engine.h"
#include "core/console/console.h"
#include "core/console/consolecommands.h"
#include "core/game/game.h"
#include "core/script/squirrel/squirrel.h"
#include "core/script/lua/lua.h"
#include "core/game/level/level.h"
#include "core/actor/components/moveablecomponent/moveablecomponent.h"
#include "core/actor/camera/camera.h"
#include "core/window.h"
#include "core/actor/pawn/pawn.h"
#include "core/renderer/renderer.h"

namespace loki
{

namespace
{

//////////////////////////////////////////////////////////////////////////
// Scans a string for 'true', 'TRUE', 'false' and 'FALSE'.
// Returns 0 if it matches a false case, 1 if it matches a true case.
// If none of the cases is found, returns 3.
//////////////////////////////////////////////////////////////////////////
int ScanStringForBoolean( const std::string& _Str )
{
	if (!_Str.compare("true") || !_Str.compare("TRUE"))
	{
		return (int)true;
	}
	else if (!_Str.compare("false") || !_Str.compare("FALSE"))
	{
		return (int)false;
	}
	return 3;
}

bool ScanValueForBoolean( LkConsole::Value _V )
{
	bool b = false;
	int i = ScanStringForBoolean(_V.m_String);
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
	LkConsole::CommandResult res("Engine version: v%i.%i.%i ('%s')", loki::VersionMajor, loki::VersionMinor, loki::VersionBuild, loki::VersionName);
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

CONSOLE_FUNCTION(Console_GetPlayerID)
{
	LkConsole::CommandResult res("Command does not do anything yet");
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
	float s = g_Engine->GetEngineUpTime();	// Seconds.
	float m = s / 60.0f;	// Minutes.
	float h = s / 3600.0f;	// Hours.

	int h2 = (int)h;
	int m2 = (int)(m - ((int)h * 60));
	float s2 = s - (h2 * 3600) - (m2 * 60);

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
	util::GetTimeStamp(s);
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
	float x = _Command->m_Arguments[0].m_Float;
	float y = _Command->m_Arguments[1].m_Float;
	float z = _Command->m_Arguments[2].m_Float;
	g_Engine->GetGame()->GetLevel()->GetCurrentCamera()->GetComponent<LkMoveableComponent>()->SetPosition(glm::vec3(x, y, z));
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_CamGetPos)
{
	glm::vec3 p = g_Engine->GetGame()->GetLevel()->GetCurrentCamera()->GetComponent<LkMoveableComponent>()->GetPosition();
	return LkConsole::CommandResult("CamPos: [%f, %f, %f]", p.x, p.y, p.z);
}

CONSOLE_FUNCTION(Console_CamSetOrientation)
{
	float x = _Command->m_Arguments[0].m_Float;
	float y = _Command->m_Arguments[1].m_Float;
	float z = _Command->m_Arguments[2].m_Float;
	float w = _Command->m_Arguments[3].m_Float;
	g_Engine->GetGame()->GetLevel()->GetCurrentCamera()->GetComponent<LkMoveableComponent>()->SetOrientation(glm::quat(x, y, z, w));
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_CamGetOrientation)
{
	glm::quat o = g_Engine->GetGame()->GetLevel()->GetCurrentCamera()->GetComponent<LkMoveableComponent>()->GetOrientation();
	return LkConsole::CommandResult("CamOrientation: [%f, %f, %f, %f]", o.x, o.y, o.z, o.w);	
}

CONSOLE_FUNCTION(Console_CamSetFoV)
{
	g_Engine->GetGame()->GetLevel()->GetCurrentCamera()->SetFoVY(_Command->m_Arguments[0].m_Float);
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_CamGetFoV)
{
	return LkConsole::CommandResult("CamFoV: %f", g_Engine->GetGame()->GetLevel()->GetCurrentCamera()->GetFoVY());
}

CONSOLE_FUNCTION(Console_CamSetZNear)
{
	g_Engine->GetGame()->GetLevel()->GetCurrentCamera()->SetZNear(_Command->m_Arguments[0].m_Float);
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_CamSetZFar)
{
	g_Engine->GetGame()->GetLevel()->GetCurrentCamera()->SetZFar(_Command->m_Arguments[0].m_Float);
	return LkConsole::CommandResult("");
}
CONSOLE_FUNCTION(Console_CamGetZPlanes)
{
	return LkConsole::CommandResult("Cam ZNear: %f\tZFar: %f", g_Engine->GetGame()->GetLevel()->GetCurrentCamera()->GetZNear(), g_Engine->GetGame()->GetLevel()->GetCurrentCamera()->GetZFar());
}

CONSOLE_FUNCTION(Console_SetFullscreen)
{
	LkWindow* window = g_Engine->GetWindow();
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
	LkWindow* window = g_Engine->GetWindow();
	int x = _Command->m_Arguments[0].m_Int;
	int y = _Command->m_Arguments[1].m_Int;
	window->SetPosition(x, y);
	return LkConsole::CommandResult("Set window position to [%i, %i]", x, y);
}

CONSOLE_FUNCTION(Console_SetWindowSize)
{
	LkWindow* window = g_Engine->GetWindow();
	int x = _Command->m_Arguments[0].m_Int;
	int y = _Command->m_Arguments[1].m_Int;
	window->SetDimensions(x, y);
	return LkConsole::CommandResult("Set window size to [%i, %i]", x, y);
}

CONSOLE_FUNCTION(Console_Terminate)
{
	g_Engine->RequestExit();
	return LkConsole::CommandResult("Requested termination...");
}

CONSOLE_FUNCTION(Console_PawnSpawn)
{
	if (g_Engine->GetGame()->GetLevel()->SpawnPawn(_Command->m_Arguments[0].m_String.c_str()))
	{
		return LkConsole::CommandResult("Spawned pawn '%s'", _Command->m_Arguments[0].m_String.c_str());
	}
	return LkConsole::CommandResult("Unable to spawn pawn '%s'", _Command->m_Arguments[0].m_String.c_str());
}

CONSOLE_FUNCTION(Console_PawnDespawn)
{
	LkPawn* pawn = g_Engine->GetGame()->GetLevel()->GetPawn(_Command->m_Arguments[0].m_String.c_str());
	if (!pawn)
	{
		return LkConsole::CommandResult("Pawn '%s' does not exist", _Command->m_Arguments[0].m_String.c_str());
	}
	g_Engine->GetGame()->GetLevel()->DespawnPawn(pawn);
	
	return LkConsole::CommandResult("Despawned pawn '%s'", _Command->m_Arguments[0].m_String.c_str());
}

CONSOLE_FUNCTION(Console_PawnSetPos)
{
	LkPawn* pawn = g_Engine->GetGame()->GetLevel()->GetPawn(_Command->m_Arguments[0].m_String.c_str());
	if (!pawn)
	{
		return LkConsole::CommandResult("Pawn '%s' does not exist", _Command->m_Arguments[0].m_String.c_str());
	}
	LkMoveableComponent* comp = pawn->GetComponent<LkMoveableComponent>();
	comp->SetPosition(glm::vec3(_Command->m_Arguments[1].m_Float, _Command->m_Arguments[2].m_Float, _Command->m_Arguments[3].m_Float));
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_PawnSetOri)
{
	LkPawn* pawn = g_Engine->GetGame()->GetLevel()->GetPawn(_Command->m_Arguments[0].m_String.c_str());
	if (!pawn)
	{
		return LkConsole::CommandResult("Pawn '%s' does not exist", _Command->m_Arguments[0].m_String.c_str());
	}
	LkMoveableComponent* comp = pawn->GetComponent<LkMoveableComponent>();
	comp->SetOrientation(glm::quat(_Command->m_Arguments[1].m_Float, _Command->m_Arguments[2].m_Float, _Command->m_Arguments[3].m_Float, _Command->m_Arguments[4].m_Float));
	return LkConsole::CommandResult("");
}

CONSOLE_FUNCTION(Console_GBufferTargets)
{
#ifdef _DEBUG
	renderer::g_Renderer->ToggleVisualizeRenderTargets();
	return LkConsole::CommandResult("");
#else
	return LkConsole::CommandResult("This command is only available in a debug build");
#endif
}

}	// Namespace loki.
