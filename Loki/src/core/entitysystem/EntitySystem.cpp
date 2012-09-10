#include "core/entitysystem/EntitySystem.h"
#include "core/entitysystem/Entity.h"
#include "util/hash/hash_fnv/hash_fnv.h"

namespace loki
{

IEntitySystem* g_EntitySystem = 0;

IEntitySystem* CreateEntitySystem()
{
	return new EntitySystem();
}

EntitySystem::EntitySystem()
{

}

EntitySystem::~EntitySystem()
{

}

IEntity* EntitySystem::FindEntityByName( const char* _EntityName ) const
{
	EntityID id = GenerateEntityIDFromName(_EntityName);
	return FindEntityByID(id);
}

IEntity* EntitySystem::FindEntityByID( EntityID _ID ) const
{
	EntitiesConstIter it = m_Entities.find(_ID);
	if (it != m_Entities.end())
	{
		return (*it).second;
	}
	return 0;
}

IEntity* EntitySystem::SpawnEntity( const char* _EntityName )
{
	IEntity* entity = FindEntityByName(_EntityName);
	if (entity)
	{
		LOG(VL_ERROR, "EntitySystem::SpawnEntity: An entity named '%s' already exists", _EntityName);
		return 0;
	}
	entity = new Entity();
	((Entity*)entity)->SetID(GenerateEntityIDFromName(_EntityName));
	((Entity*)entity)->SetName(_EntityName);

	m_Entities.insert(EntitiesPair(entity->GetID(), entity));

	return entity;
}

void EntitySystem::DestroyEntity( const char* _EntityName )
{
	IEntity* entity = FindEntityByName(_EntityName);
	if (entity)
	{
		m_Entities.erase(entity->GetID());
	}
}

EntityID EntitySystem::GenerateEntityIDFromName( const char* _EntityName ) const
{
	EntityID ID;
	
	ID.m_ID = util::Hash_FNV32(_EntityName);
	
	return ID;
}

}