#pragma once

#ifndef CONSOLE_H
#define CONSOLE_H

// These includes are here for convenience, so that clients need only include console.h in order to be able to use it.
#include <list>
#include <vector>
#include <string>
#include <map>
#include "core/eventsystem/eventlistener/eventlistener.h"
#include "core/html/htmlview/htmlviewlistener.h"

namespace loki
{

class LkHTMLView;

class LkConsole	: public LkEventListener, public LkHTMLViewListener
{
public:
	class Command;
	struct CommandInput;
	class CommandResult;

	typedef CommandResult (*ConsoleCommandFunction)( CommandInput* _Command );

	LkConsole( LkEngine* _Engine );
	~LkConsole();

	//////////////////////////////////////////////////////////////////////////
	// Must be called after all other engine components (namely the browser
	// and the renderer) have been initialized. The console itself is the very 
	// first component to be initialized but it can't create it's UI or register
	// for events yet at that time so that must be done later.
	//////////////////////////////////////////////////////////////////////////
	void FinalizeInitialization();

	//////////////////////////////////////////////////////////////////////////
	// Is called before deleting the webbrowser (because of the UI that is used
	// for the console).
	//////////////////////////////////////////////////////////////////////////
	void Deinitialize();

	//////////////////////////////////////////////////////////////////////////
	// Registers the console commands. This function is required because the
	// console is no longer a class, but a namespace (to hide certain 
	// implementation details). This has the disadvantage of not offering a
	// constructor that can take care of registering console commands.
	// Therefore, this function is called when the engine starts up so that
	// calls to Console::Execute() can actually find functions to execute.
	//////////////////////////////////////////////////////////////////////////
	void RegisterCommands();

	//////////////////////////////////////////////////////////////////////////
	// Returns a list of all console commands. Can be VERY slow.
	//////////////////////////////////////////////////////////////////////////
	std::list<std::string> GetCommandsList();

	//////////////////////////////////////////////////////////////////////////
	// Returns the description of a command.
	//////////////////////////////////////////////////////////////////////////
	bool GetDescriptionOfCommand( const char* _Command, std::string& _Output );

	//////////////////////////////////////////////////////////////////////////
	// Prints a message to the console. A newline character is appended to the string.
	//////////////////////////////////////////////////////////////////////////
	void Print( const char* _Message );

	//////////////////////////////////////////////////////////////////////////
	// Render the console window.
	//////////////////////////////////////////////////////////////////////////
	void Render();

	//////////////////////////////////////////////////////////////////////////
	// Executes a console command (If the command is invalid the console can notify the user
	// of this fact). A command should follow the following convention:
	//
	// command <space> argument1 <space> argument2 <space> argument3 ... )
	//
	// NOTE: The arguments are optional and are only used if a command actually requires them.
	//
	// The console class will parse the string that is passed to it and will identify the command and
	// all of its arguments. Once the command is known, it's function pointer is looked up through the m_Commands map.
	// A CommandInput object is then instantiated and passed to this function. The function is expected to return a
	// CommandResult object which can then be used by the Console class to print a result to the console. Console commands
	// are not expected to be executed many times per frame (not even once per frame on average), which is why we simply
	// compare the strings between commands to find out which commands need to be called.
	//////////////////////////////////////////////////////////////////////////
	void Execute( const char* _Command );

	LkEngine* GetEngine();

	void ReloadUI();

	bool ToggleVisible();

	void onFinishLoading(Awesomium::WebView* caller);

	struct Value
	{
		union
		{
			int m_Int;
			bool m_Boolean;
		};
		float m_Float;	// Float must be separated because the data will be messed up if it's not. This does
		// mean that the data structure occupies 4 additional bytes.
		std::string m_String;
	};

	class Command
	{
	public:
		enum ArgumentType
		{
			AT_INT = 1,	// MUST NOT BE 0
			AT_BOOLEAN,
			AT_FLOAT,
			AT_STRING,
		};

		Command( const std::string& _Description, ConsoleCommandFunction _Function, ... );

		std::list<std::string> m_RequiredArguments;
		ConsoleCommandFunction m_Function;
		std::string m_Description;
	private:
		Command();	// Private default c-tor.

		friend class std::map<std::string, Command>;
	};

	struct CommandInput
	{
		std::string m_Command;
		std::vector<Value> m_Arguments;	
	};

	class CommandResult
	{
	public:
		CommandResult( char* _Message, ... );
		std::string m_Result;

	private:
		CommandResult();	// Private default c-tor.
	};
private:
	LkConsole();

	void _Init();

	void _OnEvent( const LkEvent& _Event );

	LkEngine* m_Engine;
	//std::list<std::string> m_Buffer;
	//std::string	m_InputBuffer;
	std::map<std::string, Command> m_Commands;
	CRITICAL_SECTION m_CriticalSection;

	LkHTMLView* m_ConsoleUI;
	bool m_IsVisible;
};

extern LkConsole* g_Console;

}	// Namespace loki.

#endif