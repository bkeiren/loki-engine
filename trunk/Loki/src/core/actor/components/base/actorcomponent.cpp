#include "core/actor/components/base/actorcomponent.h"

namespace loki
{

LkActorComponent::LkActorComponent()	:
	m_Actor(NULL)
{

}

LkActorComponent::~LkActorComponent()
{

}

void LkActorComponent::SetActor( LkActor* _Actor )
{
	assert(_Actor != NULL);
	assert(m_Actor == NULL);	// If m_Actor is already set, something went wrong. Components should only get their actor's
								// set once (When they're created by an actor).
	m_Actor = _Actor;
}

LkActor* LkActorComponent::GetActor()
{
	return m_Actor;
}

void LkActorComponent::_OnEvent( const LkEvent& _Event )
{

}

}