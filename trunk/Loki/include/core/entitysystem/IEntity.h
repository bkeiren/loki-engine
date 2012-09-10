#pragma once

#ifndef IENTITY_H
#define IENTITY_H

#include "core/transform/Transform.h"

namespace loki
{

class IPhysicsObject;
class ICharacterObject;
class IAIObject;
class IStaticObject;

struct EntityID
{
	EntityID();
	
	glm::uint32 m_ID;

	bool operator == ( EntityID _ID ) const;
	bool operator != ( EntityID _ID ) const;
	bool operator < ( EntityID _ID ) const;
	bool operator > ( EntityID _ID ) const;
	bool operator <= ( EntityID _ID ) const;
	bool operator >= ( EntityID _ID ) const;
};

class IEntity
{
public:
	virtual IPhysicsObject* GetPhysicsObject() const = 0;
	virtual void SetPhysicsObject( IPhysicsObject* _PhysicsObject ) = 0;
	
	virtual ICharacterObject* GetCharacterObject() const = 0;
	virtual void SetCharacterObject( ICharacterObject* _CharacterObject ) = 0;
	
	virtual IAIObject* GetAIObject() const = 0;
	virtual void SetAIObject( IAIObject* _AIObject ) = 0;
	
	virtual IStaticObject* GetStaticObject() const = 0;
	virtual void SetStaticObject( IStaticObject* _StaticObject ) = 0;

	virtual EntityID GetID() const = 0;

	virtual const std::string& GetName() const = 0;

	virtual const Transform& GetTransform() const = 0;
protected:
	IEntity();
	virtual ~IEntity() = 0;

private:
	virtual void SetName( const char* _Name ) = 0;

	virtual void SetID( EntityID _ID ) = 0;
};

}

#endif