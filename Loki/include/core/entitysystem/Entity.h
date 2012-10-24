#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "core/transform/Transform.h"
#include "util/typeinfo/typeinfo.h"

#define USE_HASH_MAP

#ifdef USE_HASH_MAP
#include <hash_map>
#define MAP_TYPE	stdext::hash_map
#else
#include <map>
#define MAP_TYPE	std::map
#endif

#define RTTI_TYPEID			0
#define RTTI_DYNAMIC_CAST	1

// Used to control which way is used
// to compare types of components. Either by 
// using the typeid() operator (RTTI_TYPEID)
// or by using dynamic_cast<>() (RTTI_DYNAMIC_CAST).
#define RTTI_TYPE			RTTI_TYPEID


namespace loki
{

class Component;

struct EntityID
{
	EntityID();

	uint32 m_ID;

	bool operator == ( const EntityID& _ID ) const;
	bool operator != ( const EntityID& _ID ) const;

	// These <, >, <= and >= operators are implemented so that EntityID can be used as a key for maps.
	bool operator < ( const EntityID& _ID ) const;
	bool operator > ( const EntityID& _ID ) const;
	bool operator <= ( const EntityID& _ID ) const;
	bool operator >= ( const EntityID& _ID ) const;
};

class Entity
{
	friend class EntitySystem;
	
#if RTTI_TYPE == RTTI_TYPEID
	typedef MAP_TYPE<util::TypeInfo, Component*>	Components;	// Pointers to std::type_info are safe because they are valid throughout the entire application lifetime.
	typedef std::pair<util::TypeInfo, Component*>	ComponentsPair;	// ^
#elif RTTI_TYPE == RTTI_DYNAMIC_CAST
	CONTAINER_MACRO_LIST(Component*, Components);
#endif
public:
	EntityID GetID() const;

	const std::string& GetName() const;

	const Transform& GetTransform() const;
	Transform& GetTransform();

	//////////////////////////////////////////////////////////////////////////
	// Adds a component to an entity. Calling this function requires a template
	// syntax with the type of the component as argument. For example, if
	// an entity is to receive a component of type MoveableComponent, this
	// function can be called like so:
	// entity::InstantiateComponent<MoveableComponent>();
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	void InstantiateComponent();

	//////////////////////////////////////////////////////////////////////////
	// Removes a component from the entity.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	void RemoveComponent();

	//////////////////////////////////////////////////////////////////////////
	// Checks whether the entity has a component of a certain type.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	bool HasComponent() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns a pointer to a component of the entity has one of the specified
	// type, otherwise NULL.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	_ComponentType* GetComponent() const;
private:
	Entity();
	~Entity();

	void SetName( const char* _Name );

	void SetID( EntityID _ID );

	//////////////////////////////////////////////////////////////////////////
	// Removes all components.
	//////////////////////////////////////////////////////////////////////////
	void _ClearComponents();

	EntityID m_EntityID;
	std::string m_Name;

	Transform m_Transform;

	Components m_Components;
};

}

#include "core/entitysystem/Entity.inl"

#endif