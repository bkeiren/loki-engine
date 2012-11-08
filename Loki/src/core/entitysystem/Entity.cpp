#include "core/entitysystem/Entity.h"
#include "core/entitysystem/component/Component.h"
#include "core/entitysystem/component/default/Transform.h"

namespace loki
{

EntityID::EntityID()	:
	m_ID(0)
{

}

bool EntityID::operator == ( const EntityID& _ID ) const
{
	return (m_ID == _ID.m_ID);
}

bool EntityID::operator != ( const EntityID& _ID ) const
{
	return (m_ID != _ID.m_ID);
}

bool EntityID::operator < ( const EntityID& _ID ) const
{
	return (m_ID < _ID.m_ID);
}

bool EntityID::operator > ( const EntityID& _ID ) const
{
	return (m_ID > _ID.m_ID);
}

bool EntityID::operator <= ( const EntityID& _ID ) const
{
	return (m_ID <= _ID.m_ID);
}

bool EntityID::operator >= ( const EntityID& _ID ) const
{
	return (m_ID >= _ID.m_ID);
}

Entity::Entity()	:
	m_Transform(0)
{
	// An entity always has a transform component.
	m_Transform = InstantiateComponent<Transform>();
}

Entity::~Entity()
{
	_ClearComponents();
}

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
	return *m_Transform;
}

Transform& Entity::GetTransform()
{
	// NOTE: Post event to check translation in scene management system?
	// There is also a version of GetTransform which returns a const reference, so hopefully the compiler will use that version
	// in places where the transform is only read and not modified. That will ensure that if we only want to read the transform
	// we won't place an unnecessary event for this entity.
	return *m_Transform;
}

void Entity::SetName( const char* _Name )
{
	m_Name = std::string(_Name);
}

void Entity::SetID( EntityID _ID )
{
	m_EntityID = _ID;
}

void Entity::_ClearComponents()
{
	for (Components::iterator it = m_Components.begin(); it != m_Components.end(); ++it)
	{
#if RTTI_TYPE == RTTI_TYPEID
		delete (*it).second;
#elif RTTI_TYPE == RTTI_DYNAMIC_CAST
		delete (*it);
#endif
	}
	m_Components.clear();
}

}