#pragma once

#ifndef ACTOR_H
#define ACTOR_H

#define USE_HASH_MAP

#ifdef USE_HASH_MAP
#include <hash_map>
#define MAP_TYPE	stdext::hash_map
#else
#include <map>
#define MAP_TYPE	std::map
#endif
#include "core/eventsystem/eventmanager.h"
#include "core/eventsystem/eventlistener/eventlistener.h"
#include "util/typeinfo/typeinfo.h"

namespace loki
{

namespace game
{

class LkLevel;

}

class LkActorComponent;

typedef unsigned int ActorID;

//////////////////////////////////////////////////////////////////////////
// Base class for all game objects. 
//////////////////////////////////////////////////////////////////////////
class LkActor	: public LkEventListener
{
	friend void LkEventManager::Post( const LkEvent& _Event );

	typedef MAP_TYPE<util::TypeInfo, LkActorComponent*>	ComponentMap;	// Pointers to std::type_info are safe because they are valid throughout the entire application lifetime.
	typedef std::pair<util::TypeInfo, LkActorComponent*>	ComponentPair;	// ^
public:
	friend class LkPawn;	// TODO: Remove (?).
	friend class LkLight;
	friend class LkCamera;
	friend class game::LkLevel;

	//////////////////////////////////////////////////////////////////////////
	// This function hashes an actor name and returns the hashed value.
	// Can be used to find what hash value a certain name corresponds to.
	//////////////////////////////////////////////////////////////////////////
	static unsigned int GetHashForName( const char* _Name ); 

	//////////////////////////////////////////////////////////////////////////
	// Returns the actor name.
	//////////////////////////////////////////////////////////////////////////
	const std::string& GetName() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns the actor ID.
	//////////////////////////////////////////////////////////////////////////
	const ActorID GetID() const;

	//////////////////////////////////////////////////////////////////////////
	// Constructs and returns an std::string object that represents the
	// actor's data. Inheriting classes are allowed to write their own 
	// implementations of this function, but it is recommended to also call
	// this base version as it returns the string representation
	// of the base 'Actor' part of the object.
	// Example: Class TestActor should call Actor::ToString() and append it's
	// own string to whatever Actor::ToString() returned.
	//////////////////////////////////////////////////////////////////////////
	virtual std::string ToString() const;

	//////////////////////////////////////////////////////////////////////////
	// Adds a component to an actor. Calling this function requires a template
	// syntax with the type of the component as argument. For example, if
	// an actor is to receive a component of type MoveableComponent, this
	// function can be called like so:
	// Actor::AddComponent<MoveableComponent>();
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	void AddComponent();

	//////////////////////////////////////////////////////////////////////////
	// Removes a component from the actor.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	void RemoveComponent();

	//////////////////////////////////////////////////////////////////////////
	// Checks whether the actor has a component of a certain type.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	bool HasComponent() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns a pointer to a component of the actor has one of the specified
	// type, otherwise NULL.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	_ComponentType* GetComponent() const;
private:
	LkActor( const char* _Name, game::LkLevel* _Level );
	LkActor();	// Private default c-tor.
	virtual ~LkActor() = 0;	// '= 0' to make the class abstract.

	//////////////////////////////////////////////////////////////////////////
	// Removes all components.
	//////////////////////////////////////////////////////////////////////////
	void _ClearComponents();

	//////////////////////////////////////////////////////////////////////////
	// Gets called by the physics when actors touch.
	//////////////////////////////////////////////////////////////////////////
	virtual void _Touch( LkActor* _Actor );

	//////////////////////////////////////////////////////////////////////////
	// Gets called by the physics when actors stop touching.
	//////////////////////////////////////////////////////////////////////////
	virtual void _Untouch( LkActor* _Actor );

	virtual void _OnEvent( const LkEvent& _Event );
protected:
	std::string m_Name;				// Actor name represented as a string.
	ActorID m_ID;					// Actor ID. This ID is obtained by hashing the Actor name.

	game::LkLevel* m_Level;			// Pointer to the level that this Actor is in.
									// This can be used by the Actor (and all classes that inherit from it) to
									// access things such as the list(s) of Actors.

	ComponentMap m_Components;
};

}

#include "core/actor/actor.inl"

#endif