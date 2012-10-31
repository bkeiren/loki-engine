#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include "core/eventsystem/eventlistener/eventlistener.h"
#include "util/typeinfo/typeinfo.h"

namespace loki
{

#define DECLARE_COMPONENT_TYPEINFO(componentclass)		static util::general::TypeInfo& GetTypeInfo() { static util::general::TypeInfo ti = util::general::TypeInfo(typeid(componentclass)); return ti; }

class Entity;

//////////////////////////////////////////////////////////////////////////
// The Component class is the base class for any components that 
// an entity could be composed of.
// Examples for component types are: PhysicsComponent, RenderComponent,
// MoveableComponent.
// !!!	IMPLEMENTATION NOTE:
//		In order to speed up type-info generation, some awesome stuff
//		is done using a macro and static member functions and such.
//		When making your own component, be sure to put the 
//		DECLARE_COMPONENT_TYPEINFO macro in the public field of the class,
//		passing your component class type as argument.
//		Without this macro, the class will yield compiler errors telling you
//		that 'GetTypeInfo' is not a member of your class!
//		Example:
//		class MyComponent : public Component
//		{
//		public:
//			DECLARE_COMPONENT_TYPEINFO
//		};
//		Note that you should not use this macro in any custom 'base' component
//		classes. For instance, say you want to have a base component class
//		to provide functionality that a number of additional child components
//		would use. In that case you would only use this macro on the child classes
//		because it's of no use to do so on the base class (Although it should not
//		provide errors just be safe and try to avoid it ;) ).
// !!!
//////////////////////////////////////////////////////////////////////////
class Component	: public LkEventListener
{
	friend class Entity;
public:
	inline Entity* GetEntity();
	inline const Entity* GetEntity() const;

protected:
	Component();
	virtual ~Component() = 0;

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