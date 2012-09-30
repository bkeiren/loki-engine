#pragma once

#ifndef IENTITYSYSTEM_H
#define IENTITYSYSTEM_H

#include "core/entitysystem/IEntity.h"
#include "core/frustum/Frustum.h"

namespace loki
{

class IEntitySystem
{
public:
	CONTAINER_MACRO_LIST(IEntity*, EntitiesList);

	IEntitySystem();
	virtual ~IEntitySystem() = 0;

	virtual IEntity* FindEntityByName( const char* _EntityName ) const = 0;

	virtual IEntity* FindEntityByID( EntityID _ID ) const = 0;

	virtual IEntity* SpawnEntity( const char* _EntityName ) = 0;

	virtual void DestroyEntity( const char* _EntityName ) = 0;

	virtual void FindEntitiesInRange( const vec3& _Center, float _Range, EntitiesList& _OutputList ) const = 0;
	virtual void FindEntitiesInFrustum( const Frustum& _Frustum, const mat4& _FrustumTransform, EntitiesList& _OutputList ) const = 0;
private:
	virtual EntityID GenerateEntityIDFromName( const char* _EntityName ) const = 0;
};

IEntitySystem* CreateEntitySystem();

extern IEntitySystem* g_EntitySystem;

}

#endif