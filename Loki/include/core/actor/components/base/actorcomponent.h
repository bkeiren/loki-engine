#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include "core/eventsystem/eventlistener/eventlistener.h"

namespace loki
{

enum EComponentType
{
	CT_RENDERCOMPONENT = 0,
	CT_MOVEABLECOMPONENT,

	CT_COUNT
};

//////////////////////////////////////////////////////////////////////////
// The ActorComponent class is the base class for any components that 
// an actor should be composed of.
// Examples for component types are: PhysicsComponent, RenderComponent,
// MoveableComponent.
//////////////////////////////////////////////////////////////////////////
class LkActorComponent	: public LkEventListener
{
	friend class LkActor;
public:
protected:
	LkActorComponent();
	virtual ~LkActorComponent() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Called on each tick.
	//////////////////////////////////////////////////////////////////////////
	virtual void Update() = 0;

	LkActor* GetActor();
private:
	void SetActor( LkActor* _Actor );

	virtual void _OnEvent( const LkEvent& _Event );

	//////////////////////////////////////////////////////////////////////////
	// The actor to which this component belongs.
	//////////////////////////////////////////////////////////////////////////
	LkActor* m_Actor;	
};

}

#endif