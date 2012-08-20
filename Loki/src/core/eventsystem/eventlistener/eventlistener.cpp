#include "core/eventsystem/eventlistener/eventlistener.h"

namespace loki
{

LkEventListener::LkEventListener()
{

}

LkEventListener::~LkEventListener()
{

}

void LkEventListener::PostEvent( const LkEvent& _Event )
{
	g_EventManager->Post(_Event);
}

void LkEventListener::SubscribeToEvent( EventType _Event )
{
	g_EventManager->SubscribeToEvent(_Event, this);
}

void LkEventListener::UnsubscribeFromEvent( EventType _Event )
{
	g_EventManager->UnsubscribeFromEvent(_Event, this);
}

}