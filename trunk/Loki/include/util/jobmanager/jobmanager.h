#pragma once

#ifndef JOBMANAGER_H
#define JOBMANAGER_H

#include <Windows.h>
#include <list>
#include "util/jobmanager/job.h"

namespace loki
{

namespace util
{

namespace general
{

class Thread;
class Job;

typedef std::list<Job*> JobList;

class JobManager
{
public:
	//////////////////////////////////////////////////////////////////////////
	// Instantiates a job manager with _NumThreads number of threads and 
	// _MaxNumJobs number of maximum jobs to be waiting at any time.
	// _Sleep indicates whether the process should sleep for a very short time
	// right after creating all threads. This could potentially be useful
	// for allowing debug output to be printed before other messages 
	// (which might not have anything to do with the job manager) become
	// visible. If _Sleep is set to 0 (default), the process does not sleep.
	//////////////////////////////////////////////////////////////////////////
	JobManager( uint32 _NumThreads, uint32 _MaxNumJobs );

	//////////////////////////////////////////////////////////////////////////
	// WARNING: The destructor waits for all current job threads to finish
	// executing their current job and then stops them.
	// This is NOT an asynchronous function and depending on the number
	// of jobs and/or the current workload, this function may stall
	// your program. BE AWARE OF THIS!
	// The stalling might be avoided by calling this function in it's own thread
	// entirely.
	//////////////////////////////////////////////////////////////////////////
	~JobManager();

	//////////////////////////////////////////////////////////////////////////
	// Adds a job to be executed in the future.
	//////////////////////////////////////////////////////////////////////////
	void AddJob( Job* _Job );

	//////////////////////////////////////////////////////////////////////////
	// Adds a number of jobs from an array of pointers to be executed in the 
	// future. This is a convenience function that simply utilizes AddJob() 
	// and adds each job in the array (up to _NumJobs) sequentially.
	//////////////////////////////////////////////////////////////////////////
	void AddJobs( Job** _JobArray, uint32 _NumJobs );

	//////////////////////////////////////////////////////////////////////////
	// Returns the number of pending jobs.
	//////////////////////////////////////////////////////////////////////////
	int32 GetNumPendingJobs();

	//////////////////////////////////////////////////////////////////////////
	// Returns the number of threads that the jobmanager can utilize.
	//////////////////////////////////////////////////////////////////////////
	uint32 GetNumThreads();
private:
	//////////////////////////////////////////////////////////////////////////
	// Waits until a job is available and then returns it. Returns NULL
	// if the thread that asked for a job has to stop.
	//////////////////////////////////////////////////////////////////////////
	Job* GetNextJob();

	struct ThreadData
	{
		int32 m_Index;
		JobManager* m_JobManager;
	};

	JobManager();	// Default c-tor.

	//////////////////////////////////////////////////////////////////////////
	// For internal use. Stops all job-threads and waits for all of them to
	// finish.
	//////////////////////////////////////////////////////////////////////////
	void StopThreads();

	//////////////////////////////////////////////////////////////////////////
	// Thread entry point.
	//////////////////////////////////////////////////////////////////////////
	static DWORD WINAPI ThreadEntry( LPVOID _Parameter );

	HANDLE* m_ThreadHandles;
	HANDLE* m_Handles;
	HANDLE* m_ExitHandles;
	CRITICAL_SECTION m_JobListCritSec;
	uint32 m_NumThreads;
	uint32 m_MaxNumJobs;
	
	JobList m_JobList;
};

extern JobManager* g_JobManager;

}

}

}

#endif