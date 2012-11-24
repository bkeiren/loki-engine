#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "core/entitysystem/component/default/Transform.h"
#include "util/typeinfo/typeinfo.h"

#define USE_HASH_MAP

#ifdef USE_HASH_MAP
#include <hash_map>
#endif

namespace loki
{

class Component;

namespace components
{
	class CameraComponent;
	class Light;
	class MeshRenderer;
}

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

	CONTAINER_MACRO_VECTOR(Component*, ComponentsVector)
	
#ifdef USE_HASH_MAP
	CONTAINER_MACRO_HASH_MAP(util::general::TypeInfo, ComponentsVector, Components)
#else
	CONTAINER_MACRO_MAP(util::general::TypeInfo, ComponentsVector, Components)
#endif	
public:
	EntityID GetID() const;

	const std::string& GetName() const;

	//////////////////////////////////////////////////////////////////////////
	// Synonymous to GetComponent<type>(), except
	// that GetComponent<>() searches for the Transform component in the 
	// component map, while GetTransform, GetCamera, GetLight etc. simply return 
	// the already cached instances. This means that these functions are faster than
	// GetComponent<>().
	//////////////////////////////////////////////////////////////////////////
	const Transform& GetTransform() const;
	Transform& GetTransform();

	const components::CameraComponent* GetCamera() const;
	components::CameraComponent* GetCamera();

	const components::Light* GetLight() const;
	components::Light* GetLight();

	const components::MeshRenderer* GetRenderer() const;
	components::MeshRenderer* GetRenderer();

	//PROPERTY(components::CameraComponent, Entity, camera, {return *self.m_Camera;}, {});

	//////////////////////////////////////////////////////////////////////////
	// Adds a component to an entity. Calling this function requires a template
	// syntax with the type of the component as argument. For example, if
	// an entity is to receive a component of type MyComponent, this
	// function can be called like so:
	// entity::InstantiateComponent<MyComponent>();
	// Multiple instances of the same type can be added unless that component
	// type was declared using DECLARE_COMPONENT_SINGLE_INSTANCE.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	_ComponentType* InstantiateComponent();

	//////////////////////////////////////////////////////////////////////////
	// Removes a component from the entity.
	// Removes the most recently added component if more than
	// one component of the type passed have been added.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	void RemoveComponent();

	//////////////////////////////////////////////////////////////////////////
	// Removes a specific component instance from the entity.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	void RemoveComponent( _ComponentType* _Instance );

	//////////////////////////////////////////////////////////////////////////
	// Removes all components of the passed type.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	void RemoveComponents();

	//////////////////////////////////////////////////////////////////////////
	// Transform specialization.
	//////////////////////////////////////////////////////////////////////////
	template<>
	inline void RemoveComponent<Transform>();

	//////////////////////////////////////////////////////////////////////////
	// Checks whether the entity has a component of a certain type.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	bool HasComponent() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns a pointer to a component of the entity has one of the specified
	// type, otherwise NULL. If more than one component of the passed
	// type have been added, returns the most recently added one.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	_ComponentType* GetComponent() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns the vector of components of the passed type, if at least one
	// component of such type has been added. If no components of
	// the specified type have been added, returns NULL.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	const std::vector<_ComponentType*>* GetComponents() const;	

	//////////////////////////////////////////////////////////////////////////
	// Transform specialization.
	//////////////////////////////////////////////////////////////////////////
	template<>
	inline Transform* GetComponent<Transform>() const;
private:
	Entity();
	~Entity();

	void SetName( const char* _Name );

	void SetID( EntityID _ID );

	public:
	static Component* _CreateComponentByTypeName( const std::string& _Name );
	private:

	//////////////////////////////////////////////////////////////////////////
	// Removes all components.
	//////////////////////////////////////////////////////////////////////////
	void _ClearComponents();

	//////////////////////////////////////////////////////////////////////////
	// Private callback that is called when a component is added to the entity.
	// Can be specialized for component types to provide specific functionality
	// for that type. This is called AFTER the component is added to the entity
	// but BEFORE it is initialized.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	void _OnComponentInstantiated( _ComponentType* _Component );

	// _OnComponentInstantiated specializations.
#define ONINSTANTIATESPEC(type)	template<> void Entity::_OnComponentInstantiated<type>( type* _Component )
	ONINSTANTIATESPEC(Transform);
	ONINSTANTIATESPEC(components::CameraComponent);
	ONINSTANTIATESPEC(components::Light);
	ONINSTANTIATESPEC(components::MeshRenderer);

	//////////////////////////////////////////////////////////////////////////
	// Same as _OnComponentInstantiated, but for when a component is removed.
	// This is called AFTER the component has been terminated but BEFORE
	// it's memory is freed.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	void _OnComponentRemoved( _ComponentType* _Component );

	// _OnComponentRemoved specializations.
#define ONREMOVESPEC(type)	template<> void Entity::_OnComponentRemoved<type>( type* _Component )
	ONREMOVESPEC(Transform);
	ONREMOVESPEC(components::CameraComponent);
	ONREMOVESPEC(components::Light);
	ONREMOVESPEC(components::MeshRenderer);

	EntityID m_EntityID;
	std::string m_Name;

	Components m_Components;

	// Commonly used components are stored here for ease of access.
	// The _OnComponentInstantiated() specializations store the pointers and
	// the _OnComponentRemoved() specializations clear them to 0.
	Transform* m_Transform;
	components::CameraComponent* m_Camera;
	components::Light* m_Light;
	components::MeshRenderer* m_Renderer;
};

}

#include "core/entitysystem/Entity.inl"

#undef ONINSTANTIATESPEC
#undef ONREMOVESPEC

#endif