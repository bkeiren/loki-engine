#pragma once

#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include "core/entitysystem/IEntitySystem.h"

namespace loki
{

class EntitySystem	: public IEntitySystem
{
	friend IEntitySystem* CreateEntitySystem();

	CONTAINER_MACRO_MAP(EntityID, IEntity*, Entities);
public:
	IEntity* FindEntityByName( const char* _EntityName ) const;

	IEntity* FindEntityByID( EntityID _ID ) const;

	IEntity* SpawnEntity( const char* _EntityName );

	void DestroyEntity( const char* _EntityName );
private:
	EntitySystem();
	~EntitySystem();

	EntityID GenerateEntityIDFromName( const char* _EntityName ) const;

	Entities m_Entities;
};

}

#endif