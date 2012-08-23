#pragma once

#ifndef EVENT_H
#define EVENT_H

namespace loki
{

//////////////////////////////////////////////////////////////////////////
// Engine-defined event types as integers. These start at INT_MAX - 1 so
// client events can start enumerating at 0. (Which might be simpler to
// use and see/debug for clients).
//////////////////////////////////////////////////////////////////////////
#define EVENT_BASE			(INT_MAX)	// Do not use as event type.

//////////////////////////////////////////////////////////////////////////
// Clients can use this define as the base from which they start enumerating
// their own events. An example would be to declare an enum where
// the first value is assigned EVENT_CLIENT_BASE.
// Example:
// enum
// {
//		EVENT_CUSTOMEVENT = EVENT_CLIENT_BASE,
//		EVENT_ANOTHEREVENT,
//		EVENT_SOMETHIRDEVENT
// };
#define EVENT_CLIENT_BASE	0

#define EVENT_FRAMESTART			(EVENT_BASE - 1)
#define EVENT_FRAMEEND				(EVENT_BASE - 2)
#define EVENT_PREUPDATE				(EVENT_BASE - 3)
#define EVENT_POSTUPDATE			(EVENT_BASE - 4)
#define EVENT_PRERENDER				(EVENT_BASE - 5)
#define EVENT_POSTRENDER			(EVENT_BASE - 6)
#define EVENT_ONUPDATE				(EVENT_BASE - 7)
#define EVENT_MOUSEMOVE				(EVENT_BASE - 8)
#define EVENT_MOUSEWHEELMOVE		(EVENT_BASE - 9)
#define EVENT_MB_LEFT_DOWN			(EVENT_BASE - 10)
#define EVENT_MB_LEFT_PRESSED		(EVENT_BASE - 11)
#define EVENT_MB_LEFT_RELEASED		(EVENT_BASE - 12)
#define EVENT_MB_RIGHT_DOWN			(EVENT_BASE - 13)
#define EVENT_MB_RIGHT_PRESSED		(EVENT_BASE - 14)
#define EVENT_MB_RIGHT_RELEASED		(EVENT_BASE - 15)
#define EVENT_MB_MIDDLE_DOWN		(EVENT_BASE - 16)
#define EVENT_MB_MIDDLE_PRESSED		(EVENT_BASE - 17)
#define EVENT_MB_MIDDLE_RELEASED	(EVENT_BASE - 18)
#define EVENT_WINDOWRESIZE			(EVENT_BASE - 19)
#define EVENT_PREPHYSICSUPDATE		(EVENT_BASE - 20)
#define EVENT_POSTPHYSICSUPDATE		(EVENT_BASE - 21)

typedef unsigned int	EventType;

class LkEvent
{
#define DATASIZE	16
public:
	LkEvent( EventType _Type );
	~LkEvent();

	EventType GetEventType() const;

	void** m_Data[DATASIZE];
private:
	LkEvent();

	EventType m_Type;
protected:
};

}

#endif