#include "core/time/Time.h"

namespace loki
{

Time* g_Time = 0;

Time::Time()	:
	m_FrameCount(0),
	m_FrameTime(1.0f),
	m_ScaledFrameTime(1.0f),
	m_TimeScale(1.0f)
{
	m_GlobalClock.Start();
}

Time::~Time()
{

}

uint32 Time::GetFrameCount() const
{
	return m_FrameCount;
}

f32 Time::GetFrameTime() const
{
	return m_ScaledFrameTime;
}

f32 Time::GetActualFrameTime() const
{
	return m_FrameTime;
}

f32 Time::GetGlobalTime() const
{
	return m_GlobalClock.Lap();
}

f32 Time::GetTimeScale() const
{
	return m_TimeScale;
}

void Time::SetTimeScale( f32 _TimeScale )
{
	m_TimeScale = _TimeScale;
}

void Time::_PrepareFrameTime()
{
	m_FrameClock.Start();
}

void Time::_CalculateFrameTime()
{
	m_FrameTime = m_FrameClock.Lap();
	m_ScaledFrameTime = m_FrameTime * m_TimeScale;
	++m_FrameCount;
}

void Time::_SetFrameTime( f32 _FrameTime )
{
	m_FrameTime = _FrameTime;
}

}