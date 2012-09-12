#include "core/entitysystem/IEntity.h"

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

IEntity::IEntity()
{

}

IEntity::~IEntity()
{

}

}