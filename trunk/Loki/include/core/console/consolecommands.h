#pragma once

#ifndef CONSOLECOMMANDS_H
#define CONSOLECOMMANDS_H

namespace loki
{

// Function prototype:
// CommandResult Function( CommandInput* _Command );

#define CONSOLE_FUNCTION(name)	LkConsole::CommandResult name( LkConsole::CommandInput* _Command )

CONSOLE_FUNCTION(Console_GetEngineVersion);
CONSOLE_FUNCTION(Console_GetGameVersion);
CONSOLE_FUNCTION(Console_PrintText);
CONSOLE_FUNCTION(Console_Godmode);
CONSOLE_FUNCTION(Console_Noclip);
CONSOLE_FUNCTION(Console_PrintAllCommands);
CONSOLE_FUNCTION(Console_ReloadConsoleUI);
CONSOLE_FUNCTION(Console_GetEngineUpTime);
CONSOLE_FUNCTION(Console_GetFrameRate);
CONSOLE_FUNCTION(Console_GetTimeStamp);
CONSOLE_FUNCTION(Console_CloseConsole);
CONSOLE_FUNCTION(Console_BGConsole);
CONSOLE_FUNCTION(Console_CommandDescription);
CONSOLE_FUNCTION(Console_SquirrelRunScript);
CONSOLE_FUNCTION(Console_SquirrelRunScriptAsync);
CONSOLE_FUNCTION(Console_LuaRunScript);
CONSOLE_FUNCTION(Console_LuaRunScriptAsync);
CONSOLE_FUNCTION(Console_LuaRunString);
CONSOLE_FUNCTION(Console_LuaRunStringAsync);
CONSOLE_FUNCTION(Console_CamSetPos);
CONSOLE_FUNCTION(Console_CamGetPos);
CONSOLE_FUNCTION(Console_CamSetOrientation);
CONSOLE_FUNCTION(Console_CamGetOrientation);
CONSOLE_FUNCTION(Console_CamSetFoV);
CONSOLE_FUNCTION(Console_CamGetFoV);
CONSOLE_FUNCTION(Console_CamSetZNear);
CONSOLE_FUNCTION(Console_CamSetZFar);
CONSOLE_FUNCTION(Console_CamGetZPlanes);
CONSOLE_FUNCTION(Console_SetFullscreen);
CONSOLE_FUNCTION(Console_SetWindowPos);
CONSOLE_FUNCTION(Console_SetWindowSize);
CONSOLE_FUNCTION(Console_Terminate);
CONSOLE_FUNCTION(Console_GBufferTargets);
CONSOLE_FUNCTION(Console_LightVolumes);

}	// Namespace loki.

#endif