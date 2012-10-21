#pragma once

#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "core/eventsystem/eventmanager.h"

namespace loki
{

class LkEventListener
{
	friend class LkEventManager;

	CONTAINER_MACRO_LIST(EventType, Subscriptions);
public:
	LkEventListener();
	virtual ~LkEventListener();

	void PostEvent( const LkEvent& _Event );
	void SubscribeToEvent( EventType _Event );
	void UnsubscribeFromEvent( EventType _Event );
private:
	virtual void _OnEvent( const LkEvent& _Event ) = 0;

	// The event listener keeps an internal list of all event
	// types that it has registered to, so that it can unregister
	// from all of these when the object is destructed.
	Subscriptions m_Subscriptions;
};

}

#endif