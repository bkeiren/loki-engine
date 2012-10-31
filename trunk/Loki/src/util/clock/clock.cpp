#include <sys/timeb.h>
#include "util/clock/clock.h"

using namespace loki;
using namespace loki::util;
using namespace loki::util::time;

#ifdef WIN32
LARGE_INTEGER Clock::m_Frequency;
#endif

Clock::Clock()
#ifndef WIN32
	: m_StartCount(GetMilliCount())
#endif
{
#ifdef WIN32
	QueryPerformanceFrequency(&m_Frequency);
#endif
}

Clock::~Clock()
{

}

int32 Clock::GetMilliCount()
{
#ifdef WIN32
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	return int32((double)temp.QuadPart / (double)m_Frequency.QuadPart);
#else
	// http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm (Accessed 18-12-2011 @ 21:10 - Can be used freely)
	// Something like GetTickCount but portable
	// It rolls over every ~ 12.1 days (0x100000/24/60/60)
	// Use GetMilliSpan to correct for rollover
	timeb tb;
	ftime( &tb );
	int32 nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
#endif
}

int32 Clock::GetMilliSpan( int32 nTimeStart )
{
#ifdef WIN32
	return (GetMilliCount() - nTimeStart);
	//return (GetTickCount() - nTimeStart);
#else
	// http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm (Accessed 18-12-2011 @ 21:10 - Can be used freely)
	int32 nSpan = GetMilliCount() - nTimeStart;
	if ( nSpan < 0 )
		nSpan += 0x100000 * 1000;
	return nSpan;
#endif
}

void Clock::Start()
{
#ifdef WIN32
	QueryPerformanceCounter(&m_Start);
#else
	m_StartCount = GetMilliCount();
#endif
}

// f32 Clock::Stop()
// {
// 	return (Stop_ms() * 0.001f);
// }
// 
// int32 Clock::Stop_ms()
// {
// 	return GetMilliSpan(m_StartCount);
// }

f32 Clock::Lap() const
{
	return (Lap_ms() * 0.001f);
}

f32 Clock::Lap_ms() const
{
#ifdef WIN32
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	return f32( end.QuadPart - m_Start.QuadPart ) / f32( m_Frequency.QuadPart * 0.001f );
#else
	return GetMilliSpan(m_StartCount);
#endif
}