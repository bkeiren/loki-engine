#include "core/script/lua/lua.h"
#include "util/thread/thread.h"

namespace loki
{

LkLua* g_Lua = NULL;

// lua_State* Lua::m_State = NULL;
// CRITICAL_SECTION Lua::m_CriticalSection;
// std::queue<Lua::QueueItem*> Lua::m_Queue;
// HANDLE Lua::m_QueueSemaphoreHandle;
// HANDLE Lua::m_ThreadStopEventHandle;
// HANDLE Lua::m_ThreadExittedHandle;
// util::Thread* Lua::m_Thread = NULL;

LkLua::LkLua()	:
	m_State(NULL),
	m_Thread(NULL)
{
	_Init();
}

LkLua::~LkLua()
{
	_Shutdown();
}

bool LkLua::_Init()
{
	InitializeCriticalSection(&m_CriticalSection);
	m_QueueSemaphoreHandle = CreateSemaphore(NULL, 0, 512, NULL);
	m_ThreadStopEventHandle = CreateEvent(NULL, true, false, NULL);
	m_ThreadExittedHandle = CreateEvent(NULL, true, false, NULL);

	m_State = lua_newstate(LuaAlloc, NULL);
	if (!m_State)
	{
		LOG(VL_ERROR, "Lua::Init: Failed to initialize Lua state");
		INIT_FAIL("Lua");
	}

	// Register functions.
	RegisterFunction("LOG", LuaLog);

	// Start thread function.
	m_Thread = new util::LkThread(Thread, (void*)this, 0);

	LOG(VL_ALWAYS, "Lua::Init: State initialized");
	return true;
}

void LkLua::_Shutdown()
{
	// Indicate that the script thread should stop.
	SetEvent(m_ThreadStopEventHandle);
	// The semaphores for the queue is also released because 
	// that will allow the thread to continue (even though it's queue
	// might be empty. In order to keep the thread from checking for an 
	// item in the queue, the function has been designed in such a way that
	// it will return if necessary, before attempting to
	// obtain any item from the queue.
	ReleaseSemaphore(m_QueueSemaphoreHandle, 1, NULL);

	// Wait until the thread has finished.
	WaitForSingleObject(m_ThreadExittedHandle, INFINITE);

	// Ensure the thread is properly cleaned up.
	// (Even though the thread is manually 
	// stopped and cleaned up within the function).
	if (m_Thread)
	{
		m_Thread->Stop();
	}

	CloseHandle(m_ThreadExittedHandle);
	CloseHandle(m_ThreadStopEventHandle);
	CloseHandle(m_QueueSemaphoreHandle);
	DeleteCriticalSection(&m_CriticalSection);


	// If no state has been created, don't try to terminate.
	if (m_State)
	{
		// Close Lua.
		lua_close(m_State);
	}

	LOG(VL_ALWAYS, "Lua::Shutdown: Done");
}

void LkLua::RunScript( const char* _File )
{
	luaL_dofile(m_State, _File);
}

void LkLua::RunString( const char* _String )
{
	luaL_dostring(m_State, _String);
}

void LkLua::RunScriptAsync( const char* _File )
{
	QueueItem* item = new QueueItem();
	item->m_String = std::string(_File);
	item->m_Type = 0;

	// Push a script to the queue.
	EnterCriticalSection(&m_CriticalSection);
	m_Queue.push(item);
	LeaveCriticalSection(&m_CriticalSection);

	// Increase the semaphore count by 1.
	ReleaseSemaphore(m_QueueSemaphoreHandle, 1, NULL);
}

void LkLua::RunStringAsync( const char* _String )
{
	QueueItem* item = new QueueItem();
	item->m_String = std::string(_String);
	item->m_Type = 1;

	// Push a script to the queue.
	EnterCriticalSection(&m_CriticalSection);
	m_Queue.push(item);
	LeaveCriticalSection(&m_CriticalSection);

	// Increase the semaphore count by 1.
	ReleaseSemaphore(m_QueueSemaphoreHandle, 1, NULL);
}

void LkLua::RegisterFunction( const char* _Name, lua_CFunction _Function )
{
	lua_register(m_State, _Name, _Function);
}

void* LkLua::LuaAlloc( void* _Ud, void* _Ptr, size_t _osize, size_t _nsize )
{
	// Simple allocation implementation obtained
	// from http://pgl.yoyo.org/luai/i/lua_Alloc.
	if (_nsize == 0) 
	{
		free(_Ptr);
		return NULL;
	}
	else
	{
		return realloc(_Ptr, _nsize);
	}
}

int32 LkLua::LuaLog( lua_State* _L )
{
	const char *Arg = lua_tostring(_L, 1);
	LOG(VL_NORMAL, "Lua: %s", Arg);

	return 0;
}

int32 LkLua::Thread( void* _Arg )
{
	LkLua* instance = (LkLua*)_Arg;

	// Wait for a script or string to be available in the queue.
	WaitForSingleObject(instance->m_QueueSemaphoreHandle, INFINITE);

	// Check whether the thread should stop.
	if (WaitForSingleObject(instance->m_ThreadStopEventHandle, 0) != WAIT_TIMEOUT)
	{
		// Signal the event to indicate this thread is going to be stopped.
		SetEvent(instance->m_ThreadExittedHandle);
		return THREAD_RETURN_EXIT;
	}

#ifdef _DEBUG
	if (instance->m_Queue.size() <= 0)
	{
		LOG(VL_ERROR, "Lua::Thread: Queue is empty but thread was signaled");
		return THREAD_RETURN_AGAIN;
	}
#endif

	// Take a script from the top of the queue.
	EnterCriticalSection(&instance->m_CriticalSection);
	QueueItem* item = instance->m_Queue.front();
	instance->m_Queue.pop();
	LeaveCriticalSection(&instance->m_CriticalSection);

	// Execute the script or string.
	if (item->m_Type == 0)
	{
		instance->RunScript(const_cast<char*>(item->m_String.c_str()));
	}
	else
	{
		instance->RunString(const_cast<char*>(item->m_String.c_str()));
	}

	delete item;

	return THREAD_RETURN_AGAIN;
}

}