#pragma once

#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "core/eventsystem/eventmanager.h"

namespace loki
{

class LkEventListener
{
	friend class LkEventManager;
public:
	LkEventListener();
	virtual ~LkEventListener();

	void PostEvent( const LkEvent& _Event );
	void SubscribeToEvent( EventType _Event );
	void UnsubscribeFromEvent( EventType _Event );
private:
	virtual void _OnEvent( const LkEvent& _Event ) = 0;
};

}

#endif