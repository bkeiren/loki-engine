#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include "core/entitysystem/IEntity.h"

namespace loki
{

class Entity	: public IEntity
{
	friend class EntitySystem;
public:
	IPhysicsObject* GetPhysicsObject() const;
	void SetPhysicsObject( IPhysicsObject* _PhysicsObject );

	ICharacterObject* GetCharacterObject() const;
	void SetCharacterObject( ICharacterObject* _CharacterObject );

	IAIObject* GetAIObject() const;
	void SetAIObject( IAIObject* _AIObject );

	IStaticObject* GetStaticObject() const;
	void SetStaticObject( IStaticObject* _StaticObject );

	EntityID GetID() const;

	const std::string& GetName() const;

	const Transform& GetTransform() const;
private:
	Entity();
	~Entity();

	void SetName( const char* _Name );

	void SetID( EntityID _ID );

	IPhysicsObject* m_PhysicsObject;
	ICharacterObject* m_CharacterObject;
	IAIObject* m_AIObject;
	IStaticObject* m_StaticObject;

	EntityID m_EntityID;
	std::string m_Name;

	Transform m_Transform;
};

}

#endif