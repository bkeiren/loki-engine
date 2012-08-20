#include "util/jobmanager/job.h"

namespace loki
{

namespace util
{

namespace
{
	JobID JobIDCounter = 0;	
}

LkJob::LkJob()	:
	m_State(JS_PENDING),
	m_ID(++JobIDCounter),
	m_EventDone(CreateEventA(NULL, true, false, NULL))
{

}

LkJob::~LkJob()
{
	CloseHandle(m_EventDone);
}

void LkJob::AddDependency( LkJob* _Dependency )
{
	assert(_Dependency != NULL);

	if (m_State != JS_PENDING)
	{
		LOG(VL_WARN, "Job::AddDependency: Job %s. Can't add dependency at this time.", (m_State == JS_RUNNING)?("is running"):((m_State == JS_DONE)?("has finished"):("is not pending")));
	}

	m_Dependencies.push_back(_Dependency);
}

void LkJob::Reset()
{
	ResetEvent(m_EventDone);
	m_State = JS_PENDING;
}

EJobState LkJob::GetState()
{
	return m_State;
}

void LkJob::WaitForJob( unsigned long _Ms /*= INFINITE*/ )
{
	WaitForSingleObject(m_EventDone, _Ms);
}

JobID LkJob::GetJobID()
{
	return m_ID;
}

void LkJob::JobMainWrapper()
{
	LOG(VL_NORMAL, "Job::JobMainWrapper: Starting job...");

	m_State = JS_RUNNING;

	JobMain();

	m_State = JS_DONE;
	
	// Signal the 'done'-event.
	SetEvent(m_EventDone);

	LOG(VL_NORMAL, "Job::JobMainWrapper: Job done.");
}

bool LkJob::CanStart()
{
	if (m_State != JS_PENDING)
	{
		return false;
	}

	for (JobList::iterator job_it = m_Dependencies.begin(); job_it != m_Dependencies.end(); ++job_it)
	{
		LkJob* job = (*job_it);

		// If any of the dependencies is not done yet, the job can't be started.
		if (job->GetState() != JS_DONE)
		{
			return false;
		}
	}
	return true;
}

}

}