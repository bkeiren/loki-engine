#include <sys/timeb.h>
#include "util/clock/clock.h"

using namespace loki::util;

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

int Clock::GetMilliCount()
{
#ifdef WIN32
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	return int((double)temp.QuadPart / (double)m_Frequency.QuadPart);
#else
	// http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm (Accessed 18-12-2011 @ 21:10 - Can be used freely)
	// Something like GetTickCount but portable
	// It rolls over every ~ 12.1 days (0x100000/24/60/60)
	// Use GetMilliSpan to correct for rollover
	timeb tb;
	ftime( &tb );
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
#endif
}

int Clock::GetMilliSpan( int nTimeStart )
{
#ifdef WIN32
	return (GetMilliCount() - nTimeStart);
	//return (GetTickCount() - nTimeStart);
#else
	// http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm (Accessed 18-12-2011 @ 21:10 - Can be used freely)
	int nSpan = GetMilliCount() - nTimeStart;
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

// float Clock::Stop()
// {
// 	return (Stop_ms() * 0.001f);
// }
// 
// int Clock::Stop_ms()
// {
// 	return GetMilliSpan(m_StartCount);
// }

float Clock::Lap() const
{
	return (Lap_ms() * 0.001f);
}

float Clock::Lap_ms() const
{
#ifdef WIN32
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	return float( end.QuadPart - m_Start.QuadPart ) / float( m_Frequency.QuadPart * 0.001f );
#else
	return GetMilliSpan(m_StartCount);
#endif
}