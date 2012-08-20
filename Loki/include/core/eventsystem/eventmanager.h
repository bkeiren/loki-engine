#pragma once

#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <list>
#include <hash_map>
#include "core/eventsystem/event/event.h"

namespace loki
{

class LkActor;
class LkEventListener;

class LkEventManager
{
	typedef std::list<LkEventListener*>						ListenersList;
	typedef stdext::hash_map<EventType, ListenersList>		Listeners;
public:
	LkEventManager();
	~LkEventManager();

	//////////////////////////////////////////////////////////////////////////
	// Post a message. Will be sent to any actors that are listening to
	// the specific event type.
	//////////////////////////////////////////////////////////////////////////
	void Post( const LkEvent& _Event );

	//////////////////////////////////////////////////////////////////////////
	// Register/unregister an actor for a specific event type.
	//////////////////////////////////////////////////////////////////////////
	void SubscribeToEvent( EventType _Type, LkEventListener* _Listener );
	void UnsubscribeFromEvent( EventType _Type, LkEventListener* _Listener );
private:
	bool _Init();
	void _Shutdown();

	Listeners m_Listeners;
};

extern LkEventManager* g_EventManager;

}

#endif