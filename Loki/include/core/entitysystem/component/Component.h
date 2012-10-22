#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include "core/eventsystem/eventlistener/eventlistener.h"
#include "core/entitysystem/Entity.h"

namespace loki
{

//////////////////////////////////////////////////////////////////////////
// The Component class is the base class for any components that 
// an entity could be composed of.
// Examples for component types are: PhysicsComponent, RenderComponent,
// MoveableComponent.
//////////////////////////////////////////////////////////////////////////
class Component	: public LkEventListener
{
	friend class Entity;
public:
protected:
	Component();
	virtual ~Component() = 0;

	inline Entity* GetEntity();

private:
	void SetEntity( Entity* _Entity );

	virtual void _OnEvent( const LkEvent& _Event );

	//////////////////////////////////////////////////////////////////////////
	// Called after data such as m_Entity is set.
	//////////////////////////////////////////////////////////////////////////
	virtual void _Init() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Called before the component is deleted.
	//////////////////////////////////////////////////////////////////////////
	virtual void _Terminate() = 0;

	//////////////////////////////////////////////////////////////////////////
	// The entity to which this component belongs.
	//////////////////////////////////////////////////////////////////////////
	Entity* m_Entity;
};

}

#include "core/entitysystem/component/Component.inl"

#endif