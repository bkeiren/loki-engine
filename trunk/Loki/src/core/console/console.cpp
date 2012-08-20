#include <cstdarg>
#include "util/clock/clock.h"
#include "core/console/console.h"
#include "core/console/consolecommands.h"
#include <Windows.h>
#include "core/html/htmlcore.h"
#include "core/renderer/renderer.h"

using namespace loki;

namespace loki
{

void JavascriptConsoleExecute( const JSArguments& _Args )
{
	std::string str = util::ToMultiByteString(_Args[0].toString());
	g_Console->Execute(str.c_str());
}

LkConsole* g_Console = NULL;

LkConsole::LkConsole( LkEngine* _Engine )	:
	m_Engine(_Engine),
	m_ConsoleUI(0),
	m_IsVisible(false)
{
	assert(m_Engine != NULL);

	_Init();
}

LkConsole::LkConsole()
{

}

LkConsole::~LkConsole()
{

}

void LkConsole::FinalizeInitialization()
{
	SubscribeToEvent(EVENT_ONUPDATE);

	m_ConsoleUI = g_HTMLCore->CreateView(renderer::g_Renderer->GetRenderWidth(), 400);
	m_ConsoleUI->SetListener(this);
	ReloadUI();
}

void LkConsole::Deinitialize()
{
	g_HTMLCore->DestroyView(&m_ConsoleUI);
	m_ConsoleUI = 0;	// DestroyView sets the pointer to 1 instead of 0, for some reason...
}

void LkConsole::_Init()
{
	InitializeCriticalSection(&m_CriticalSection);
	RegisterCommands();
}

void LkConsole::_OnEvent( const LkEvent& _Event )
{
	switch (_Event.GetEventType())
	{
	case EVENT_ONUPDATE:
		{
			if (KEY_RELEASED(KEY_TILDE))
			{
				ToggleVisible();
			}

			if (m_IsVisible)
			{
				//g_HTMLCore->SetFocus(m_ConsoleUI);

				// Make sure the input field always has focus while the console is open.
				m_ConsoleUI->ExecuteJavascript("jQuery('#console_input').focus();");
			}

			break;
		}
	}
}

void LkConsole::RegisterCommands()
{
	// The key to the map is the string representation of the command that needs to be
	// entered by the user in order to call a function. The value is the address of the function that is to be called.
	// Be aware that adding elements in this way may overwrite key-value pairs. This means that if you add to the map
	// with a key that has already been used, the address of the function will be overwritten.
	
	m_Commands["getengineversion"] = Command("Provides the current engine version", &Console_GetEngineVersion);
	m_Commands["getgameversion"] = Command("Provides the current game version and title", &Console_GetGameVersion);
	m_Commands["getplayerid"] = Command("Provides the player's actor ID", &Console_GetPlayerID);
	m_Commands["print"] = Command("Prints text to the console", &Console_PrintText, Command::AT_STRING);
	m_Commands["godmode"] = Command("Turns godmode on or off", &Console_Godmode, Command::AT_BOOLEAN);
	m_Commands["noclip"] = Command("Turns clipping on or off", &Console_Noclip, Command::AT_BOOLEAN);
	m_Commands["printcommands"] = Command("Prints a list of commands, synonymous to 'help'", &Console_PrintAllCommands);
	m_Commands["help"] = Command("Prints a list of commands, synonymous to 'printcommands'", &Console_PrintAllCommands);
	m_Commands["reloadconsoleui"] = Command("Reloads the console UI's resource file. Allows for on-the-fly changes to the UI without restarting the engine", &Console_ReloadConsoleUI);
	m_Commands["getengineuptime"] = Command("Provides the duration for which the engine has been running", &Console_GetEngineUpTime);
	m_Commands["getframerate"] = Command("Provides the current framerate and whether the framecap is enabled", &Console_GetFrameRate);
	m_Commands["gettimestamp"] = Command("Provides a time stamp at the time of execution", &Console_GetTimeStamp);
	m_Commands["close"] = Command("Closes the console window", &Console_CloseConsole);
	m_Commands["bgconsole"] = Command("Hides or shows the background console window of the operating system", &Console_BGConsole, Command::AT_BOOLEAN);
	m_Commands["desc"] = Command("Provides a description of a console command", &Console_CommandDescription, Command::AT_STRING);
	m_Commands["sq_rscript"] = Command("Runs a Squirrel script", &Console_SquirrelRunScript, Command::AT_STRING);
	m_Commands["sq_rscripta"] = Command("Runs a Squirrel script asynchronously", &Console_SquirrelRunScriptAsync, Command::AT_STRING);
	m_Commands["lua_rscript"] = Command("Runs a Lua script", &Console_LuaRunScript, Command::AT_STRING);
	m_Commands["lua_rscripta"] = Command("Runs a Lua script asynchronously", &Console_LuaRunScriptAsync, Command::AT_STRING);
	m_Commands["lua_rstring"] = Command("Runs a Lua string", &Console_LuaRunString, Command::AT_STRING);
	m_Commands["lua_rstringa"] = Command("Runs a Lua string asynchronously", &Console_LuaRunStringAsync, Command::AT_STRING);
	m_Commands["cam.setpos"] = Command("Sets the current camera's position", &Console_CamSetPos, Command::AT_FLOAT, Command::AT_FLOAT, Command::AT_FLOAT);
	m_Commands["cam.getpos"] = Command("Gets the current camera's position", &Console_CamGetPos);
	m_Commands["cam.setori"] = Command("Sets the current camera's orientation from a quaternion", &Console_CamSetOrientation, Command::AT_FLOAT, Command::AT_FLOAT, Command::AT_FLOAT, Command::AT_FLOAT);
	m_Commands["cam.getori"] = Command("Gets the current camera's orientation as a quaternion", &Console_CamGetOrientation);
	m_Commands["cam.setfov"] = Command("Sets the current camera's field of view", &Console_CamSetFoV, Command::AT_FLOAT);
	m_Commands["cam.getfov"] = Command("Gets the current camera's field of view", &Console_CamGetFoV);
	m_Commands["cam.setznear"] = Command("Sets the current camera's z-near distance", &Console_CamSetZNear, Command::AT_FLOAT);
	m_Commands["cam.setzfar"] = Command("Sets the current camera's z-far distance", &Console_CamSetZFar, Command::AT_FLOAT);
	m_Commands["cam.getzplanes"] = Command("Gets the current camera's z-distances (both near and far)", &Console_CamGetZPlanes);
	m_Commands["fullscreen"] = Command("Sets the window to fullscreen or windowed mode. Synonymous to 'win.fullscr'", &Console_SetFullscreen, Command::AT_BOOLEAN);
	m_Commands["win.fullscr"] = Command("Sets the window to fullscreen or windowed mode. Synonymous to 'fullscreen'", &Console_SetFullscreen, Command::AT_BOOLEAN);
	m_Commands["win.setpos"] = Command("Sets the window position", &Console_SetWindowPos, Command::AT_INT, Command::AT_INT);
	m_Commands["win.setsize"] = Command("Sets the window size", &Console_SetWindowSize, Command::AT_INT, Command::AT_INT);
	m_Commands["terminate"] = Command("Signals for termination of the game and the engine", &Console_Terminate);
}

std::list<std::string> LkConsole::GetCommandsList()
{
	std::list<std::string> list;
	for (std::map<std::string, Command>::iterator it = m_Commands.begin(); it != m_Commands.end(); ++it)
	{
		std::string cmd = (*it).first;
		for (std::list<std::string>::iterator it2 = (*it).second.m_RequiredArguments.begin(); it2 != (*it).second.m_RequiredArguments.end(); ++it2)
		{
			cmd += " <";
			cmd += (*it2);
			cmd += ">";
		}
		list.push_back(cmd);
	}
	return list;
}

bool LkConsole::GetDescriptionOfCommand( const char* _Command, std::string& _Output )
{
	std::map<std::string, Command>::iterator it = m_Commands.find(_Command);
	if (it != m_Commands.end())
	{
		_Output = (*it).second.m_Description;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// Prints a message to the console, a new line character is appended.
// _Message is not const since sprintf() is used to 
//////////////////////////////////////////////////////////////////////////
void LkConsole::Print( const char* _Message )
{	
	if (m_ConsoleUI)
	{
		std::string msg = _Message;
		util::StringReplaceAll(msg, "\"", "&#34;");

		std::string js = "var c = jQuery(\"#console_textarea\");c.append(\">";
		js += msg;
		js += "\\n\");c.animate({scrollTop:c[0].scrollHeight - c.height()}, 200, null);";

		// Replace certain characters because javascript and HTML have different characters-escape 
		// requirements than C++.
		util::StringReplaceAll(js, "\n", "\\n");
		util::StringReplaceAll(js, "\t", "\\t");
		util::StringReplaceAll(js, "<", "&lt;");
		util::StringReplaceAll(js, ">", "&gt;");

		EnterCriticalSection(&m_CriticalSection);
		m_ConsoleUI->ExecuteJavascript(js);
		LeaveCriticalSection(&m_CriticalSection);
	}
}

void LkConsole::Render()
{
	if (m_IsVisible)
	{
		m_ConsoleUI->Render();
	}
}

//////////////////////////////////////////////////////////////////////////
// Executes a console command.
//////////////////////////////////////////////////////////////////////////
void LkConsole::Execute( const char* _Command )
{
	LOG(VL_COMMAND, _Command);

	std::string str = _Command;
	std::string arg;

	CommandInput command;
	int idx = str.find(' ');
	command.m_Command = str.substr(0, idx);

	if (idx > -1)	// Keep the code from adding or parsing values when no arguments are passed.
	{
		str.erase(0, idx + 1);

		do
		{
			Value val;

			char first = str[0];
			if (first == '\"' || first == '\'')		// If the string starts with a " or a ' symbol, we assume a string starts at this point.
			{
				str.erase(0, 1);
				idx = str.find(first);
				arg = str.substr(0, idx);
				str.erase(0, idx + 2);
			}
			else
			{
				idx = str.find(' ');
				arg = str.substr(0, idx);
				str.erase(0, idx + 1);
			}

			val.m_Int = atoi(arg.c_str());
			val.m_Float = (float)atof(arg.c_str());
			val.m_String = arg;

			command.m_Arguments.push_back(val);
			
		} while (idx > -1);	// idx gets set to -1 if std::string::find can't find the next space (And thus the string has ended).
	}

	std::map<std::string, Command>::iterator it = m_Commands.find(command.m_Command);
	if (it != m_Commands.end())
	{
		if (command.m_Arguments.size() >= it->second.m_RequiredArguments.size())
		{
			util::Clock clock;
			clock.Start();

			CommandResult res = (*it).second.m_Function(&command);

			if (res.m_Result != "")
			{
				LOG(VL_NORMAL, ">> %s -- (%.3f ms)", res.m_Result.c_str(), clock.Lap_ms());
			}
		}
		else
		{
			str = "Insufficient arguments. Signature:\n\t";
			str += command.m_Command;
			for (std::list<std::string>::iterator it2 = it->second.m_RequiredArguments.begin(); it2 != it->second.m_RequiredArguments.end(); ++it2)
			{
				str += " <";
				str += (*it2);
				str += ">";
			}

			LOG(VL_COMMAND, "%s", str.c_str());
		}
	}
	else
	{
		LOG(VL_NORMAL, "Command '%s' does not exist", command.m_Command.c_str());
	}
}

LkEngine* LkConsole::GetEngine()
{
	return m_Engine;
}

void LkConsole::ReloadUI()
{
	m_ConsoleUI->LoadFile("resources//ui//console.html");
	m_ConsoleUI->CreateJavascriptObject(L"Console");
	m_ConsoleUI->BindJSDelegate(L"Console", L"Execute", &JavascriptConsoleExecute);
}

bool LkConsole::ToggleVisible()
{
	m_IsVisible = !m_IsVisible;
	m_ConsoleUI->SetActive(m_IsVisible);
	if (m_IsVisible)
	{
		g_HTMLCore->SetViewInFocus(m_ConsoleUI);
	}
	else
	{
		g_HTMLCore->SetViewInFocus(0);
	}
	return m_IsVisible;
}

void LkConsole::onFinishLoading(Awesomium::WebView* caller)
{
	// Push all auto-complete data.
	/*std::string str = "jQuery('#console_input').autocomplete(\"option\", \"source\", [";
	for (std::map<std::string, Command>::const_iterator it = m_Commands.begin(); it != m_Commands.end(); ++it)
	{
		if (it != m_Commands.begin())
		{
			str += ", ";
		}

		str += "\"";
		str += (*it).first;
		str += "\"";
	}
	str += "]);";
	m_ConsoleUI->ExecuteJavascript(str);*/
}

LkConsole::Command::Command( const std::string& _Description, ConsoleCommandFunction _Function, ... )	:
	m_Description(_Description),
	m_Function(_Function)
{
	va_list v1;
	va_start(v1, _Function);

	Command::ArgumentType type = (Command::ArgumentType)NULL;

	do 
	{
		type = (Command::ArgumentType)va_arg(v1, int);
		if (type != NULL)
		{
			char* str = NULL;
			switch (type)
			{
			case AT_INT:
				{
					str = "int";
					break;
				}
			case AT_BOOLEAN:
				{
					str = "boolean";
					break;
				}
			case AT_FLOAT:
				{
					str = "float";
					break;
				}
			case AT_STRING:
				{
					str = "string";
					break;
				}
			default:
				{
					// Don't do anything.
					continue;
				}
			}
			m_RequiredArguments.push_back(str);
		}
	} while (type != NULL);

	va_end(v1);
}

LkConsole::Command::Command()
{

}

LkConsole::CommandResult::CommandResult( char* _Message, ... )
{
	va_list v1;
	va_start(v1, _Message);

	const unsigned int msgbuffer_size = 1024;
	char msgbuffer[msgbuffer_size];

	vsnprintf_s(msgbuffer, msgbuffer_size, msgbuffer_size - 1, _Message, v1);

	m_Result = msgbuffer;

	va_end(v1);
}

LkConsole::CommandResult::CommandResult()
{

}

}	// Namespace loki..