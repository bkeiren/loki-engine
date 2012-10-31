#pragma once

#ifndef THREAD_H
#define THREAD_H

#include <Windows.h>

namespace loki
{

namespace util
{

namespace system
{

class Thread
{
public:
	// Thread creation flags.
#define THREAD_START_SUSPENDED	(1 << 0)

	// Thread return flags.
#define THREAD_RETURN_EXIT		(1 << 0)	// Stop execution.
#define THREAD_RETURN_AGAIN		(1 << 1)	// Execute again after returning.
#define THREAD_RETURN_PAUSE		(1 << 2)	// Pause the thread.
#define THREAD_EXTERNAL_EXIT	(1 << 3)	// Indicates that the thread has been asked to stop execution from outside of the thread itself.
											// Only for internal use.

	// Thread ID typedef.
	typedef uint32 ThreadID;

	// Thread function typedef.
	// Thread functions return an integer value that can be used to signal to the thread
	// class that the thread should stop executing.
	// A thread should return THREAD_RETURN_EXIT in order for this to happen.
	typedef int32(*ThreadFunc)(void*);

	typedef void(*ThreadCallback)(int32, void*, ThreadID);

	//////////////////////////////////////////////////////////////////////////
	// _Function is the thread function to execute on each iteration of
	// the thread. Functions can return values defined as THREAD_RETURN_*
	// to signal to the thread wrapper what to do after executing the function
	// once. For example, if the function returns THREAD_RETURN_EXIT, the 
	// thread will exit and clean up. If the function returns 
	// THREAD_RETURN_PAUSE the thread will pause until it's resumed by 
	// Thread::Start().
	// _Argument is a void pointer to anything that is to be passed to the
	// thread function on each iteration.
	// _Flags is a combination of any of the thread creation flags.
	// For example: By default a thread function will start executing 
	// immediately after being created. If the thread flags contain 
	// THREAD_START_SUSPENDED, the thread will not start until Thread::Start()
	// is explicitly called.
	//////////////////////////////////////////////////////////////////////////
	Thread( ThreadFunc _Function, void* _Argument, uint32 _Flags );

	//////////////////////////////////////////////////////////////////////////
	// Starts or resumes the thread's execution.
	//////////////////////////////////////////////////////////////////////////
	void Start();

	//////////////////////////////////////////////////////////////////////////
	// Suspends the thread's execution. Can be resumed by calling
	// Thread::Start().
	// WARNING: Suspending a thread in this way can cause deadlock if the
	// thread in question still holds a lock of some kind (Since it can't be
	// released by the thread while it's suspended).
	// To be safer, you could ensure that the thread's task function
	// returns THREAD_RETURN_PAUSE after it's next iteration so that the
	// thread is paused in between iterations of it's task.
	// Even though this can still cause deadlock, it's safer because
	// the thread is not potentially suspended in between
	// lock and unlock calls.
	//////////////////////////////////////////////////////////////////////////
	void Pause();

	//////////////////////////////////////////////////////////////////////////
	// Stops the thread's execution and deletes the thread object.
	// After this call, the Thread object should NOT be used anymore since
	// it is not guaranteed to still be valid.
	// It is recommended that Thread::Stop() is used instead of Thread::Kill()
	// because Thread::Kill() uses a dangerous (yet sure-fire) way of stopping
	// the thread. If a thread is possible deadlocked, Thread::Kill() is the 
	// better alternative since that will be able to kill the thread while
	// Thread::Stop() will not be able to.
	//////////////////////////////////////////////////////////////////////////
	void Stop();

	//////////////////////////////////////////////////////////////////////////
	// Deletes the thread instance and stops it's function. This function
	// kills the thread in a dangerous manner and should therefore
	// only be used if the thread is not stoppable in any other fashion (by 
	// calling Thread::Stop().
	//////////////////////////////////////////////////////////////////////////
	virtual void Kill();

	//////////////////////////////////////////////////////////////////////////
	// Returns the thread's ID.
	//////////////////////////////////////////////////////////////////////////
	const ThreadID GetID();

	//////////////////////////////////////////////////////////////////////////
	// Registers a callback to be called after each iteration of the
	// thread function. If no callback should be registerd, _Callback can be
	// 0 or NULL. The thread callback function takes 3 arguments:
	// An integer that is the return value of the last iteration of the
	// thread function. A void pointer that is the argument that was passed
	// to the thread function on it's last iteration and a ThreadID that is
	// the ID of the Thread object that called the callback.
	//////////////////////////////////////////////////////////////////////////
	void SetCallback( ThreadCallback _Callback );
private:
	Thread();	// Private default c-tor.
	virtual ~Thread();	// Thread instances shouldn't be deletable
						// from the outside. Instead, Thread::Stop() or 
						// Thread::Kill() should be used (Or the 
						// thread function should return THREAD_RETURN_EXIT.

	// Thread entry point. Static because it's address
	// needs to be accessed.
	static DWORD WINAPI EntryPoint( LPVOID _Argument );

	HANDLE			m_Handle;
	HANDLE			m_StopEventHandle;
	ThreadFunc		m_Function;
	ThreadCallback	m_Callback;
	void*			m_Argument;
	uint32	m_Flags;
	ThreadID		m_ID;
};

}

}

}

#endif