#pragma once

#ifndef CLOCK_H
#define CLOCK_H

namespace loki
{

namespace util
{

class Clock
{
public:
	Clock();
	~Clock();

	static int GetMilliCount();
	static int GetMilliSpan( int nTimeStart );

	//////////////////////////////////////////////////////////////////////////
	// Starts the timer.
	void Start();

	//////////////////////////////////////////////////////////////////////////
	// Returns the time span since Start() was called in seconds. To obtain
	// the time in milliseconds, use Stop_ms().
// 	float Stop();
// 
// 	int Stop_ms();

	//////////////////////////////////////////////////////////////////////////
	// Returns the time span since Start() was called in seconds. To obtain the time
	// in milliseconds, use Lap_ms(). NOTE: Does not reset the start time in any way.
	// In order to set a new start time, call Start().
	float Lap() const;

	float Lap_ms() const;
private:
#ifdef WIN32
	LARGE_INTEGER m_Start;
	static LARGE_INTEGER m_Frequency;
#else
	int m_StartCount;
#endif
};

}

}

#endif