#include "core/eventsystem/event/event.h"

namespace loki
{

LkEvent::LkEvent( EventType _Type )	:
	m_Type(_Type)
{
	memset(m_Data, 0, sizeof(void*) * DATASIZE);
}

LkEvent::LkEvent()
{

}

LkEvent::~LkEvent()
{

}

EventType LkEvent::GetEventType() const
{
	return m_Type;
}

}