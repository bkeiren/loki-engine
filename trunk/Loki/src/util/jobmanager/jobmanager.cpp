#include "util/jobmanager/jobmanager.h"

namespace loki
{

namespace util
{

LkJobManager* g_JobManager = NULL;

#define HANDLE_STOP			0
#define HANDLE_JOBADDED		1
#define HANDLE_JOBFINISHED	2

LkJobManager::LkJobManager( unsigned int _NumThreads, unsigned int _MaxNumJobs )	:
	m_NumThreads(_NumThreads),
	m_MaxNumJobs(_MaxNumJobs)
{
	assert(_NumThreads > 0);

	InitializeCriticalSection(&m_JobListCritSec);

	m_ThreadHandles = (HANDLE*)malloc(sizeof(HANDLE) * m_NumThreads);
	m_Handles = (HANDLE*)malloc(sizeof(HANDLE) * 3);
	m_ExitHandles = (HANDLE*)malloc(sizeof(HANDLE) * m_NumThreads);

	m_Handles[HANDLE_STOP] = CreateEvent(NULL, true, false, NULL);					// Stop event.
	m_Handles[HANDLE_JOBADDED] = CreateSemaphore(NULL, 0, m_MaxNumJobs, NULL);		// Job added to the manager semaphore.
	m_Handles[HANDLE_JOBFINISHED] = CreateSemaphore(NULL, 0, m_MaxNumJobs, NULL);	// Job finished semaphore.

	for (unsigned int i = 0; i < m_NumThreads; ++i)
	{
		ThreadData* Data = new ThreadData();
		Data->m_Index = i;
		Data->m_JobManager = this;

		m_ThreadHandles[i] = CreateThread(NULL, 0, &ThreadEntry, (void*)Data, 0, 0);
		m_ExitHandles[i] = CreateEvent(NULL, true, false, NULL);
	}
}

LkJobManager::LkJobManager()
{
	LOG(VL_ERROR, "JobManager created with default c-tor");
}

LkJobManager::~LkJobManager()
{
	StopThreads();

	for (unsigned int i = 0; i < m_NumThreads; ++i)
	{
		CloseHandle(m_ThreadHandles[i]);
		CloseHandle(m_ExitHandles[i]);
	}

	CloseHandle(m_Handles[HANDLE_STOP]);
	CloseHandle(m_Handles[HANDLE_JOBADDED]);
	CloseHandle(m_Handles[HANDLE_JOBFINISHED]);

	// Free space allocated for the dynamically allocated handle arrays.
	free(m_ThreadHandles);
	free(m_Handles);
	free(m_ExitHandles);
}

void LkJobManager::AddJob( LkJob* _Job )
{
	assert(_Job != NULL);

	EnterCriticalSection(&m_JobListCritSec);
	if (m_JobList.size() < m_MaxNumJobs)
	{
		m_JobList.push_back(_Job);
		//LOG(VL_NORMAL, "Pushed job to queue");
	}
	LeaveCriticalSection(&m_JobListCritSec);

	// Increment the job-added semaphore.
	ReleaseSemaphore(m_Handles[HANDLE_JOBADDED], 1, NULL);
}

int LkJobManager::GetNumPendingJobs()
{
	// TODO: Check whether this is thread safe!
	return m_JobList.size();
}

unsigned int LkJobManager::GetNumThreads()
{
	return m_NumThreads;
}

LkJob* LkJobManager::GetNextJob()
{
	do 
	{
		// Wait for any of the following 3 (semaphore) events:
		// - Job manager needs to exit.
		// - Job was added to the manager.
		// - Job has finished.
		// Whenever any of these events occur, this function should wake up and 
		// check whether there's a job available.
		WaitForMultipleObjects(3, m_Handles, false, INFINITE);

		if (WaitForSingleObject(m_Handles[HANDLE_STOP], 0) != WAIT_OBJECT_0)
		{		
			EnterCriticalSection(&m_JobListCritSec);
			for (JobList::iterator job_it = m_JobList.begin(); job_it != m_JobList.end(); ++job_it)
			{
				LkJob* job = (*job_it);
				if (job->CanStart())
				{
					m_JobList.remove(job);
					LeaveCriticalSection(&m_JobListCritSec);
					return job;
					break;	// Just in case, code shouldn't ever reach this point anyway.
				}
			}
			LeaveCriticalSection(&m_JobListCritSec);
		}
		else
		{
			// The above WaitForSingleObject call returns WAIT_OBJECT_0 if
			// the stop event is signaled.
			return NULL;
		}
	} while (1);

	return NULL;
}

void LkJobManager::StopThreads()
{
	// Set the stop event so that all threads will exit.
	SetEvent(m_Handles[HANDLE_STOP]);

	// Wait for all threads to actually exit.
	WaitForMultipleObjects(m_NumThreads, m_ExitHandles, true, INFINITE);
}

DWORD WINAPI LkJobManager::ThreadEntry( LPVOID _Parameter )
{
	ThreadData* Data = (ThreadData*)_Parameter;

	LOG(VL_NORMAL, "JobManager: Job thread %i started", Data->m_Index);

	while (1)
	{
		// GetNextJob() blocks until a job is available or the job manager should exit.
		LkJob* job = Data->m_JobManager->GetNextJob();

		// If GetNextJob returns NULL, the thread should stop.
		if (!job)
		{
			LOG(VL_NORMAL, "JobManager: Job thread %i exitting...", Data->m_Index);
			SetEvent(Data->m_JobManager->m_ExitHandles[Data->m_Index]);	// Set this thread's exit event to indicate the thread has actually stopped.
			ExitThread(0);
			return 0;
		}

		// Execute the job.
		job->JobMainWrapper();

		// Increment the job-finished semaphore.
		ReleaseSemaphore(Data->m_JobManager->m_Handles[HANDLE_JOBFINISHED], 1, NULL);
	}

	return 0;
}

}

}