#pragma once

#ifndef SQUIRREL_H
#define SQUIRREL_H

extern "C"
{
#ifdef _UNICODE
	#define REDEFINE_UNICODE
	#undef _UNICODE		// We won't use wide strings in our application, so we'd like
						// Squirrel not to use wide strings either (Otherwise we will have
						// to convert everything all the time. That's bad for 
						// performance and it's already caused quite some headaches).
#endif

#include "Squirrel3\\squirrel.h"

#ifdef REDEFINE_UNICODE
	#define _UNICODE	// Redefine it.
	#undef REDEFINE_UNICODE
#endif

//#include "Lua/include/lualib.h"
//#include "Lua/include/lauxlib.h"
}

#include "core/script/squirrel/squirrelscript.h"

#include <Windows.h>
#include <queue>
#include <string>

namespace loki
{

namespace util
{
	class LkThread;
}

//////////////////////////////////////////////////////////////////////////
// This class is from where Squirrel scripts can be run.
//////////////////////////////////////////////////////////////////////////
class LkSquirrel
{
	friend class LkEngine;
public:
	// static SquirrelScript* CompileScript( const char* _File );
	// static void RunScript( SquirrelScript* _Script );
	void RunScript( const char* _File );
	void RunScriptAsync( const char* _File );
	
	//////////////////////////////////////////////////////////////////////////
	// C-functions have the following prototype:
	// SQInteger FunctionName( HSQUIRRELVM );
	// A C function called from Squirrel must return either:
	// 0 : If the function doesn't return anything back to Squirrel.
	// 1 : If the function does return something to Squirrel. In such a case,
	//	   these values have to be pushed onto the stack by C (so that they
	//	   can be obtained by Squirrel for use in the calling script).
	// SQ_ERROR : If a runtime error is thrown.
	//////////////////////////////////////////////////////////////////////////
	void RegisterFunction( const char* _Name, SQFUNCTION _Function );
private:
	LkSquirrel();
	~LkSquirrel();

	bool _Init();
	void _Shutdown();

	struct QueueItem
	{
		std::string m_String;
		//int m_Type;
	};

	static SQInteger SquirrelLexRead( SQUserPointer _UserPointer );
	static SQInteger SquirrelLog( HSQUIRRELVM _VM );
	static void SquirrelRegularLog( HSQUIRRELVM _VM, const SQChar* _Msg, ... );
	static void SquirrelErrorLog( HSQUIRRELVM _VM, const SQChar* _Msg, ...);
	static void SquirrelCompilerErrorLog( HSQUIRRELVM _VM, const SQChar* _Desc, const SQChar* _Source, SQInteger _Line, SQInteger _Column );

	static int Thread( void* _Arg );

	HSQUIRRELVM m_SquirrelVM;
	CRITICAL_SECTION m_CriticalSection;
	std::queue<QueueItem*> m_Queue;
	HANDLE m_QueueSemaphoreHandle;
	HANDLE m_ThreadStopEventHandle;
	HANDLE m_ThreadExittedHandle;
	util::LkThread* m_Thread;
};

extern LkSquirrel* g_Squirrel;

}

#endif