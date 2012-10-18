#include "core/entitysystem/Entity.h"

namespace loki
{

Entity::Entity()	:
	m_PhysicsObject(0),
	/*m_CharacterObject(0),*/
	m_Model(0),
	m_AIObject(0)
	/*m_StaticObject(0)*/
{
	
}

Entity::~Entity()
{
	
}

IPhysicsObject* Entity::GetPhysicsObject() const
{
	return m_PhysicsObject;
}

void Entity::SetPhysicsObject( IPhysicsObject* _PhysicsObject )
{
	m_PhysicsObject = _PhysicsObject;
}

// ICharacterObject* Entity::GetCharacterObject() const
// {
// 	return m_CharacterObject;
// }
// 
// void Entity::SetCharacterObject( ICharacterObject* _CharacterObject )
// {
// 	m_CharacterObject = _CharacterObject;
// }

graphics::Model* Entity::GetModel() const
{
	return m_Model;
}

void Entity::SetModel( graphics::Model* _Model )
{
	m_Model = _Model;
}

IAIObject* Entity::GetAIObject() const
{
	return m_AIObject;
}

void Entity::SetAIObject( IAIObject* _AIObject )
{
	m_AIObject = _AIObject;
}

// IStaticObject* Entity::GetStaticObject() const
// {
// 	return m_StaticObject;
// }
// 
// void Entity::SetStaticObject( IStaticObject* _StaticObject )
// {
// 	m_StaticObject = _StaticObject;
// }

EntityID Entity::GetID() const
{
	return m_EntityID;
}

const std::string& Entity::GetName() const
{
	return m_Name;
}

const Transform& Entity::GetTransform() const
{
	return m_Transform;
}

Transform& Entity::GetTransform()
{
	// NOTE: Post event to check translation in scene management system?
	// There is also a version of GetTransform which returns a const reference, so hopefully the compiler will use that version
	// in places where the transform is only read and not modified. That will ensure that if we only want to read the transform
	// we won't place an unnecessary event for this entity.
	return m_Transform;
}

void Entity::SetName( const char* _Name )
{
	m_Name = std::string(_Name);
}

void Entity::SetID( EntityID _ID )
{
	m_EntityID = _ID;
}

}