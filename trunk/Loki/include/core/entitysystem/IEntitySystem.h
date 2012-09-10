#pragma once

#ifndef IENTITYSYSTEM_H
#define IENTITYSYSTEM_H

#include "core/entitysystem/IEntity.h"

namespace loki
{

class IEntitySystem
{
public:
	IEntitySystem();
	virtual ~IEntitySystem() = 0;

	virtual IEntity* FindEntityByName( const char* _EntityName ) const = 0;

	virtual IEntity* FindEntityByID( EntityID _ID ) const = 0;

	virtual IEntity* SpawnEntity( const char* _EntityName ) = 0;

	virtual void DestroyEntity( const char* _EntityName ) = 0;
private:
	virtual EntityID GenerateEntityIDFromName( const char* _EntityName ) const = 0;
};

IEntitySystem* CreateEntitySystem();

extern IEntitySystem* g_EntitySystem;

}

#endif