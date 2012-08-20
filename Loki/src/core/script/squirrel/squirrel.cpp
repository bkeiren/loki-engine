#include "core/script/squirrel/squirrel.h"
#include "SQUIRREL3\\sqstdio.h"
#include "SQUIRREL3\\sqstdaux.h"
#include "util/thread/thread.h"

namespace loki
{

LkSquirrel* g_Squirrel = NULL;

// HSQUIRRELVM Squirrel::m_SquirrelVM;
// CRITICAL_SECTION Squirrel::m_CriticalSection;
// std::queue<Squirrel::QueueItem*> Squirrel::m_Queue;
// HANDLE Squirrel::m_QueueSemaphoreHandle;
// HANDLE Squirrel::m_ThreadStopEventHandle;
// HANDLE Squirrel::m_ThreadExittedHandle;
// util::Thread* Squirrel::m_Thread = NULL;

LkSquirrel::LkSquirrel()	:
	m_Thread(NULL)
{
	_Init();
}

LkSquirrel::~LkSquirrel()
{
	_Shutdown();
}

bool LkSquirrel::_Init()
{
	InitializeCriticalSection(&m_CriticalSection);
	m_QueueSemaphoreHandle = CreateSemaphore(NULL, 0, 512, NULL);
	m_ThreadStopEventHandle = CreateEvent(NULL, true, false, NULL);
	m_ThreadExittedHandle = CreateEvent(NULL, true, false, NULL);

	// Open squirrel.
	m_SquirrelVM = sq_open(1024);

	sq_seterrorhandler(m_SquirrelVM);
	sqstd_seterrorhandlers(m_SquirrelVM);

	// Register print function and error handler.
	sq_setprintfunc(m_SquirrelVM, SquirrelRegularLog, SquirrelErrorLog);

	// Register compiler error handler.
	sq_setcompilererrorhandler(m_SquirrelVM, SquirrelCompilerErrorLog);

	
	// Register the LOG() function.
	RegisterFunction("LOG", SquirrelLog);

	// Start thread function.
	m_Thread = new util::LkThread(Thread, (void*)this, 0);

	//////////////////////////////////////////////////////////////////////////
	// Run the squirrel base script. This base script defines global functions
	// and datatypes that should be accesible in every squirrel script run
	// by the engine.
	LOG(VL_NORMAL, "Squirrel::Init: Running base script...");
	RunScript("resources//scripts//squirrel_base.nut");
	
	LOG(VL_ALWAYS, "Squirrel::Init: Initialized");
	return true;
}

void LkSquirrel::_Shutdown()
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


	// Close squirrel.
	if (m_SquirrelVM)
	{
		sq_close(m_SquirrelVM);
	}

	LOG(VL_ALWAYS, "Squirrel::Shutdown: Done");
}

// SquirrelScript* Squirrel::CompileScript( const char* _File )
// {
// 	filesystem::File* f = filesystem::OpenFile(_File);
// 	//FILE* f = fopen(_File, "rb");
// 	if (f->IsOpen())
// 	{
// 		SQRESULT res = sq_compile(m_SquirrelVM, SquirrelLexRead, (SQUserPointer)f, util::ToWideString(std::string(_File)).c_str(), true);
// 		f->Close();
// 
// 		// If compilation succeeds, the compiled function is pushed on the stack as a Squirrel function.
// 		// This functions is what sq_call() requires.
// 
// 		if (SQ_FAILED(res))
// 		{
// 			LOG(VL_ERROR, "Squirrel::CompileScript: Failed to compile Squirrel script '%s'", _File);
// 		}
// 
// 		SquirrelScript* script = new SquirrelScript();
// 		// TODO: Store Squirrel function in object.
// 		
// 		return script;
// 	}
// 	
// 	return NULL;
// }
// 
// void Squirrel::RunScript( SquirrelScript* _Script )
// {
// 	if (!_Script)
// 	{
// 		LOG(VL_ERROR, "Squirrel::RunScript: Script is null");
// 		return;
// 	}
// 
// 	//sq_call(m_SquirrelVM, SQInteger params, SQBool retval, true);
// }

void LkSquirrel::RunScript( const char* _File )
{
	sq_pushroottable(m_SquirrelVM);
	//SQRESULT res = sqstd_dofile(m_SquirrelVM, util::ToWideString(std::string(_File)).c_str(), false, true);
	SQRESULT res = sqstd_dofile(m_SquirrelVM, _File, false, true);
	if (SQ_FAILED(res))
	{
		LOG(VL_ERROR, "Squirrel::RunScript: Failed to run script:\n\t'%s'", _File);
	}
	sq_pop(m_SquirrelVM, 1);
}

void LkSquirrel::RunScriptAsync( const char* _File )
{
	QueueItem* item = new QueueItem();
	item->m_String = std::string(_File);

	// Push a script to the queue.
	EnterCriticalSection(&m_CriticalSection);
	m_Queue.push(item);
	LeaveCriticalSection(&m_CriticalSection);

	// Increase the semaphore count by 1.
	ReleaseSemaphore(m_QueueSemaphoreHandle, 1, NULL);
}

void LkSquirrel::RegisterFunction( const char* _Name, SQFUNCTION _Function )
{
	sq_pushroottable(m_SquirrelVM);
	//sq_pushstring(m_SquirrelVM, util::ToWideString(std::string(_Name)).c_str(), -1);
	sq_pushstring(m_SquirrelVM, _Name, -1);
	sq_newclosure(m_SquirrelVM, _Function, 0);
	sq_newslot(m_SquirrelVM, -3, false);
	sq_pop(m_SquirrelVM, 1);
}

SQInteger LkSquirrel::SquirrelLexRead( SQUserPointer _UserPointer )
{
	// Code obtained from http://squirrel-lang.org/doc/squirrel3.html#d0e3733.
	// Accessed 02-03-2012 @ 16:10.
	int ret;
	char c;
	if ((ret=fread(&c, sizeof(c), 1, ((filesystem::LkFile*)_UserPointer)->GetNativeFilePointer()) > 0) )
	{
		return c;
	}
	return 0;
}

SQInteger LkSquirrel::SquirrelLog( HSQUIRRELVM _VM )
{
	const char *str;
	sq_tostring(_VM, 2);
	sq_getstring(_VM, -1, &str);

	LOG(VL_NORMAL, "Squirrel: %s", str);
	
	// Function does not return a value to Squirrel. So 0 should be returned.
	// Documentation says that if the function does return a value, 1 must be returned
	// and the values must be pushed on the stack.
	// Another valid return value is SQ_ERROR which is only necessary if a
	// runtime error is thrown.
	return 0;
}

void LkSquirrel::SquirrelRegularLog( HSQUIRRELVM _VM, const SQChar* _Msg, ... )
{
	va_list v1;
	va_start(v1, _Msg);

	// Append _Msg to "Squirrel: ".
	std::string Message = "Squirrel: ";
	//Message += util::ToMultiByteString(std::wstring(_Msg));
	Message += _Msg;
	LOG(VL_NORMAL, Message.c_str(), v1);

	va_end(v1);
}

void LkSquirrel::SquirrelErrorLog( HSQUIRRELVM _VM, const SQChar* _Msg, ...)
{
	va_list v1;
	va_start(v1, _Msg);

	// Append _Msg to "Squirrel: ".
	std::string Message = "Squirrel: ";
	//Message += util::ToMultiByteString(std::wstring(_Msg));
	Message += _Msg;
	LOG(VL_ERROR, Message.c_str(), v1);

	va_end(v1);
}

void LkSquirrel::SquirrelCompilerErrorLog( HSQUIRRELVM _VM, const SQChar* _Desc, const SQChar* _Source, SQInteger _Line, SQInteger _Column )
{
	LOG(VL_ERROR, "Squirrel (COMPILER %i:%i):\nDesc: %s\nFile: %s", _Line, _Column, _Desc, _Source);
}

int LkSquirrel::Thread( void* _Arg )
{
	LkSquirrel* instance = (LkSquirrel*)_Arg;

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
//	if (item->m_Type == 0)
	{
		instance->RunScript(const_cast<char*>(item->m_String.c_str()));
	}
// 	else
// 	{
// 		instance->RunString(const_cast<char*>(item->m_String.c_str()));
// 	}

	delete item;

	return THREAD_RETURN_AGAIN;
}

}