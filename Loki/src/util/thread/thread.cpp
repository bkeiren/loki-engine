#include "util/thread/thread.h"

namespace loki
{

namespace util
{

namespace	// Anonymous namespace.
{

	LkThread::ThreadID GetNextThreadID()
	{
		static LkThread::ThreadID IDCounter = 0;
		InterlockedIncrement((long*)&IDCounter);
		return IDCounter;
	}

}

LkThread::LkThread( ThreadFunc _Function, void* _Argument, uint32 _Flags )	:
	m_Function(_Function),
	m_Callback(0),
	m_Argument(_Argument),
	m_Flags(_Flags),
	m_ID(GetNextThreadID())
{
	DWORD CreationFlags = 0;
	
	if (_Flags & THREAD_START_SUSPENDED)
	{
		CreationFlags |= CREATE_SUSPENDED;
	}

	// The thread receives a pointer to it's Thread object so that it can 
	// access data regarding the thread.
	m_Handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EntryPoint, (void*)this, CreationFlags, 0);
	m_StopEventHandle = CreateEvent(NULL, true, false, NULL);
}

LkThread::LkThread()
{

}

LkThread::~LkThread()
{
	CloseHandle(m_Handle);
	CloseHandle(m_StopEventHandle);
}

void LkThread::Start()
{
	LOG(VL_NORMAL, "Started thread with id %i", m_ID);

	ResumeThread(m_Handle);
}

void LkThread::Pause()
{
	LOG(VL_NORMAL, "Paused thread with id %i", m_ID);

	SuspendThread(m_Handle);
}

void LkThread::Stop()
{
	LOG(VL_NORMAL, "Stopped thread with id %i", m_ID);

	SetEvent(m_StopEventHandle);
}

void LkThread::Kill()
{
	TerminateThread(m_Handle, 0);	// Dangerous.
	delete this;
}

const LkThread::ThreadID LkThread::GetID()
{
	return m_ID;
}

void LkThread::SetCallback( ThreadCallback _Callback )
{
	m_Callback = _Callback;
}

DWORD WINAPI LkThread::EntryPoint( LPVOID _Argument )
{
	LkThread* threadInstance = (LkThread*)_Argument;

	while (1)
	{
		int32 result = threadInstance->m_Function(threadInstance->m_Argument);
	
		// If the thread was signaled to stop, stop it.
		if (WaitForSingleObject(threadInstance->m_StopEventHandle, 0) != WAIT_TIMEOUT)
		{
			result = THREAD_EXTERNAL_EXIT;
		}		

		// Call the thread callback function if it exists.
		if (threadInstance->m_Callback)
		{
			threadInstance->m_Callback(result, threadInstance->m_Argument, threadInstance->m_ID);
		}

		switch (result)
		{
		case THREAD_RETURN_EXIT:
		case THREAD_EXTERNAL_EXIT:
			{
				ExitThread(0);
				delete threadInstance;
				break;
			}
		case THREAD_RETURN_PAUSE:
			{
				threadInstance->Pause();
				break;
			}
		case THREAD_RETURN_AGAIN:
			{
				// Do nothing, just keep executing the function.
				break;
			}
		}
	}

	ExitThread(0);
	delete threadInstance;
	return 0;
}

}

}