#pragma once

#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include "core/entitysystem/Entity.h"
#include "core/frustum/Frustum.h"

namespace loki
{

class Entity;

class EntitySystem
{
	friend EntitySystem* CreateEntitySystem();

	CONTAINER_MACRO_MAP(EntityID, Entity*, Entities);
public:
	~EntitySystem();

	CONTAINER_MACRO_LIST(Entity*, EntitiesList);

	Entity* FindEntityByName( const char* _EntityName ) const;

	Entity* FindEntityByID( EntityID _ID ) const;

	Entity* SpawnEntity( const char* _EntityName );

	void DestroyEntity( const char* _EntityName );

	void FindEntitiesInRange( const vec3& _Center, float _Range, EntitiesList& _OutputList ) const;
	void FindEntitiesInFrustum( const Frustum& _Frustum, const mat4& _FrustumTransform, EntitiesList& _OutputList ) const;
private:
	EntitySystem();

	EntityID GenerateEntityIDFromName( const char* _EntityName ) const;

	Entities m_Entities;
};

EntitySystem* CreateEntitySystem();

extern EntitySystem* g_EntitySystem;

}

#endif