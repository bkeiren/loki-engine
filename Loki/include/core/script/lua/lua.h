#pragma once

#ifndef LUA_H
#define LUA_H

extern "C"
{
#include "Lua\\lua.h"

// Apparently these are neccesary on some
// systems.
#include "Lua\\lualib.h"
#include "Lua\\lauxlib.h"
}

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
// This class is from where Lua scripts can be run.
//////////////////////////////////////////////////////////////////////////
class LkLua
{
	friend class LkEngine;
public:
	void RunScript( const char* _File );
	void RunString( const char* _String );

	void RunScriptAsync( const char* _File );
	void RunStringAsync( const char* _String );

	//////////////////////////////////////////////////////////////////////////
	// C-functions have the following prototype:
	// int FunctionName( lua_State *L );
	//////////////////////////////////////////////////////////////////////////
	void RegisterFunction( const char* _Name, lua_CFunction _Function );
private:
	LkLua();
	~LkLua();

	bool _Init();
	void _Shutdown();

	struct QueueItem
	{
		std::string m_String;
		int m_Type;
	};

	static void* LuaAlloc( void* _Ud, void* _Ptr, size_t _osize, size_t _nsize );
	static int LuaLog( lua_State* _L );

	static int Thread( void* _Arg );

	lua_State* m_State;
	CRITICAL_SECTION m_CriticalSection;
	std::queue<QueueItem*> m_Queue;
	HANDLE m_QueueSemaphoreHandle;
	HANDLE m_ThreadStopEventHandle;
	HANDLE m_ThreadExittedHandle;
	util::LkThread* m_Thread;
};

extern LkLua* g_Lua;

}

#endif