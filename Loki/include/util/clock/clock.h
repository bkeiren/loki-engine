#pragma once

#ifndef CLOCK_H
#define CLOCK_H

namespace loki
{

namespace util
{

namespace time
{

class Clock
{
public:
	Clock();
	~Clock();

	int32 GetMilliCount();
	int32 GetMilliSpan( int32 nTimeStart );

	//////////////////////////////////////////////////////////////////////////
	// Starts the timer.
	void Start();

	//////////////////////////////////////////////////////////////////////////
	// Returns the time span since Start() was called in seconds. To obtain
	// the time in milliseconds, use Stop_ms().
// 	f32 Stop();
// 
// 	int32 Stop_ms();

	//////////////////////////////////////////////////////////////////////////
	// Returns the time span since Start() was called in seconds. To obtain the time
	// in milliseconds, use Lap_ms(). NOTE: Does not reset the start time in any way.
	// In order to set a new start time, call Start().
	f32 Lap() const;

	f32 Lap_ms() const;
private:
#ifdef WIN32
	LARGE_INTEGER m_Start;
	static LARGE_INTEGER m_Frequency;
#else
	int32 m_StartCount;
#endif
};

}

}

}

#endif