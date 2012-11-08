#include "core/entitysystem/component/Component.h"
#include "core/entitysystem/Entity.h"

namespace loki
{

Component::Component()	:
	m_Entity(0)
{

}

Component::~Component()
{

}

bool Component::IsEnabled() const
{
	return m_Enabled;
}

void Component::Enable()
{
	SetEnabled(true);
}

void Component::Disable()
{
	SetEnabled(false);
}

void Component::SetEnabled( bool _Enabled )
{
	m_Enabled = _Enabled;

	// Dispatch events for this component only. The virtual-function table chain will ensure it arrives at the right place.
	_HandleEvent( (m_Enabled) ? (EVENT_COMPONENT_ENABLED) : (EVENT_COMPONENT_DISABLED) );
}

const Transform& Component::GetTransform() const
{
	return GetEntity()->GetTransform();
}

Transform& Component::GetTransform()
{
	return GetEntity()->GetTransform();
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
	if (IsEnabled())
	{
		_HandleEvent(_Event);
	}
}

void Component::_HandleEvent( const LkEvent& _Event )
{

}

void Component::_BaseInit()
{
	_Init();
}

void Component::_BaseTerminate()
{
	_Terminate();
}

}