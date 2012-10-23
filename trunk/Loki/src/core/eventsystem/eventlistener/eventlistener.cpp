#include "core/eventsystem/eventlistener/eventlistener.h"

namespace loki
{

LkEventListener::LkEventListener()
{

}

LkEventListener::~LkEventListener()
{
	if (g_EventManager != 0)
	{
		for (SubscriptionsConstIter it = m_Subscriptions.begin(); it != m_Subscriptions.end(); ++it)
		{
			g_EventManager->UnsubscribeFromEvent((*it), this);
		}
	}
	m_Subscriptions.clear();
}

void LkEventListener::PostEvent( const LkEvent& _Event )
{
	g_EventManager->Post(_Event);
}

void LkEventListener::SubscribeToEvent( EventType _Event )
{
	g_EventManager->SubscribeToEvent(_Event, this);
	m_Subscriptions.push_back(_Event);
}

void LkEventListener::UnsubscribeFromEvent( EventType _Event )
{
	g_EventManager->UnsubscribeFromEvent(_Event, this);
	m_Subscriptions.remove(_Event);
}

}