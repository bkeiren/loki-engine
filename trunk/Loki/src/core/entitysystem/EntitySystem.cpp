#include "core/entitysystem/EntitySystem.h"
#include "core/entitysystem/Entity.h"
#include "util/hash/hash_fnv/hash_fnv.h"

namespace loki
{

#define FOREACH_ENTITY(iteratorname)	for (EntitiesConstIter iteratorname = m_Entities.begin(); iteratorname != m_Entities.end(); ++iteratorname)

EntitySystem* g_EntitySystem = 0;

EntitySystem* CreateEntitySystem()
{
	return new EntitySystem();
}

EntitySystem::EntitySystem()
{

}

EntitySystem::~EntitySystem()
{
	for (EntitiesConstIter it = m_Entities.begin(); it != m_Entities.end(); ++it)
	{
		delete (*it).second;
	}
	m_Entities.clear();
}

Entity* EntitySystem::FindEntityByName( const char* _EntityName ) const
{
	EntityID id = GenerateEntityIDFromName(_EntityName);
	return FindEntityByID(id);
}

Entity* EntitySystem::FindEntityByID( EntityID _ID ) const
{
	EntitiesConstIter it = m_Entities.find(_ID);
	if (it != m_Entities.end())
	{
		return (*it).second;
	}
	return 0;
}

Entity* EntitySystem::SpawnEntity( const char* _EntityName )
{
	Entity* entity = FindEntityByName(_EntityName);
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
	Entity* entity = FindEntityByName(_EntityName);
	if (entity)
	{
		m_Entities.erase(entity->GetID());
	}
}

void EntitySystem::FindEntitiesInRange( const vec3& _Center, f32 _Range, EntitiesList& _OutputList ) const
{

}

void EntitySystem::FindEntitiesInFrustum( const Frustum& _Frustum, const mat4& _FrustumTransform, EntitiesList& _OutputList ) const
{
	// Frustum plane normals in world-space.
	vec4 PlaneNormals[Frustum::_FRUSTUM_PLANE_COUNT] = {_Frustum.GetPlaneNormal(Frustum::FRUSTUM_PLANE_LEFT) * _FrustumTransform,
											 _Frustum.GetPlaneNormal(Frustum::FRUSTUM_PLANE_RIGHT) * _FrustumTransform,
											 _Frustum.GetPlaneNormal(Frustum::FRUSTUM_PLANE_BOTTOM) * _FrustumTransform,
											 _Frustum.GetPlaneNormal(Frustum::FRUSTUM_PLANE_TOP) * _FrustumTransform,
											 _Frustum.GetPlaneNormal(Frustum::FRUSTUM_PLANE_NEAR) * _FrustumTransform,
											 _Frustum.GetPlaneNormal(Frustum::FRUSTUM_PLANE_FAR) * _FrustumTransform};

	FOREACH_ENTITY(it)
	{
		Entity* ent = (Entity*)((*it).second);

		for (int32 i = 0; i < Frustum::_FRUSTUM_PLANE_COUNT; ++i)
		{
			// Move plane inward (along normal) by an amount equal to the radius of the bounding sphere of the
			// current entity.
			// Then check on which side of the plane the entity lies.
			// If the entity lies on the 'inside' side of each plane, it can be added to the output list.
			//PlaneNormals[i]
		}
	}
}

EntityID EntitySystem::GenerateEntityIDFromName( const char* _EntityName ) const
{
	EntityID ID;
	
	ID.m_ID = util::Hash_FNV32(_EntityName);
	
	return ID;
}

#undef FOREACH_ENTITY

}