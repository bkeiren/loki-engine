#include "util/jobmanager/job.h"

namespace loki
{

namespace util
{

namespace general
{

namespace
{
	JobID JobIDCounter = 0;	
}

Job::Job()	:
	m_State(JS_PENDING),
	m_ID(++JobIDCounter),
	m_EventDone(CreateEventA(NULL, true, false, NULL))
{

}

Job::~Job()
{
	CloseHandle(m_EventDone);
}

void Job::AddDependency( Job* _Dependency )
{
	assert(_Dependency != NULL);

	if (m_State != JS_PENDING)
	{
		LOG(VL_WARN, "Job::AddDependency: Job %s. Can't add dependency at this time.", (m_State == JS_RUNNING)?("is running"):((m_State == JS_DONE)?("has finished"):("is not pending")));
	}

	m_Dependencies.push_back(_Dependency);
}

void Job::Reset()
{
	ResetEvent(m_EventDone);
	m_State = JS_PENDING;
}

EJobState Job::GetState()
{
	return m_State;
}

void Job::WaitForJob( unsigned long _Ms /*= INFINITE*/ )
{
	WaitForSingleObject(m_EventDone, _Ms);
}

JobID Job::GetJobID()
{
	return m_ID;
}

void Job::_JobMainWrapper()
{
	LOG(VL_NORMAL, "Job::_JobMainWrapper: Starting job...");

	m_State = JS_RUNNING;

	JobMain();

	m_State = JS_DONE;
	
	// Signal the 'done'-event.
	SetEvent(m_EventDone);

	LOG(VL_NORMAL, "Job::_JobMainWrapper: Job done.");
}

bool Job::_CanStart()
{
	if (m_State != JS_PENDING)
	{
		return false;
	}

	for (JobList::iterator job_it = m_Dependencies.begin(); job_it != m_Dependencies.end(); ++job_it)
	{
		Job* job = (*job_it);

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

}