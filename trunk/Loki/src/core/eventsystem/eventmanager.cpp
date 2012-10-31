#include "core/eventsystem/eventmanager.h"
#include "core/eventsystem/eventlistener/eventlistener.h"

namespace loki
{

LkEventManager* g_EventManager = NULL;

LkEventManager::LkEventManager()
	//m_Listeners(Listeners(ET_COUNT))
{
	_Init();
}

LkEventManager::~LkEventManager()
{
	_Shutdown();
}

bool LkEventManager::_Init()
{
	LOG(VL_ALWAYS, "EventManager::Init: Event manager initialized");
	return true;
}

void LkEventManager::_Shutdown()
{
	LOG(VL_ALWAYS, "EventManager::Shutdown: Event manager shutdown");
}

void LkEventManager::Post( const LkEvent& _Event )
{
	ListenersList* actors = &m_Listeners[_Event.GetEventType()];
	for (ListenersList::iterator it = actors->begin(); it != actors->end(); ++it)
	{
		(*it)->_OnEvent(_Event);
	}
}

void LkEventManager::SubscribeToEvent( EventType _Type, LkEventListener* _Listener )
{
	m_Listeners[_Type].push_back(_Listener);
}

void LkEventManager::UnsubscribeFromEvent( EventType _Type, LkEventListener* _Listener )
{
	m_Listeners[_Type].remove(_Listener);
}

}