#pragma once

#ifndef TIME_H
#define TIME_H

#include "util/clock/clock.h"

namespace loki
{

class Time
{
	friend class LokiEngine;
public:
	uint32 GetFrameCount() const;
	f32 GetFrameTime() const;	// Affected by time scale.
	f32 GetActualFrameTime() const;	// Not affected by time scale.
	f32 GetGlobalTime() const;	// Not affected by time scale.
	f32 GetTimeScale() const;
	void SetTimeScale( f32 _TimeScale );

private:
	Time();
	~Time();

	//////////////////////////////////////////////////////////////////////////
	// To be called at start of frame.
	void _PrepareFrameTime();

	//////////////////////////////////////////////////////////////////////////
	// To be called at end of frame.
	void _CalculateFrameTime();

	void _SetFrameTime( f32 _FrameTime );
	
	uint32 m_FrameCount;	// Number of frames that have passed since the start.
	f32 m_FrameTime;	// Time it took to complete last frame (seconds).
	f32 m_ScaledFrameTime;	// m_FrameTime * m_TimeScale.
	f32 m_TimeScale;

	util::time::Clock m_FrameClock;
	util::time::Clock m_GlobalClock;
};

extern Time* g_Time;

}

#endif