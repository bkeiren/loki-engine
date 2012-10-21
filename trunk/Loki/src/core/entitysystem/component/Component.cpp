#include "core/entitysystem/component/Component.h"

namespace loki
{

Component::Component()	:
	m_Entity(0)
{

}

Component::~Component()
{

}

void Component::SetEntity( Entity* _Entity )
{
	assert(_Entity != NULL);
	assert(m_Entity == NULL);	// If m_Actor is already set, something went wrong. Components should only get their actor's
	// set once (When they're created by an actor).
	m_Entity = _Entity;
}

void Component::_OnEvent( const LkEvent& _Event )
{

}

}