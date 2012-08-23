#pragma once

#ifndef ACTOR_INL
#define ACTOR_INL

#include "core/actor/components/base/actorcomponent.h"

namespace loki
{

template< typename _ComponentType >
void LkActor::AddComponent()
{
	LkActorComponent* comp = (LkActorComponent*)new _ComponentType();
	m_Components.insert(ComponentPair(util::TypeInfo(typeid(_ComponentType)), comp));
	comp->SetActor(this);
	comp->_Init();
}

template< typename _ComponentType >
void LkActor::RemoveComponent()
{
	ComponentMap::iterator it = m_Components.find(util::TypeInfo(typeid(_ComponentType)));
	_ComponentType* comp = (_ComponentType*)((*it).second);
	m_Components.erase(it);
	delete comp;
}

template< typename _ComponentType >
bool LkActor::HasComponent() const
{
	return (m_Components.find(util::TypeInfo(typeid(_ComponentType))) != m_Components.end());
}

template< typename _ComponentType >
_ComponentType* LkActor::GetComponent() const
{
	ComponentMap::const_iterator it = m_Components.find(util::TypeInfo(typeid(_ComponentType)));
	if (it != m_Components.end())
	{
		return (_ComponentType*)((*it).second);
	}
	std::string t = typeid(_ComponentType).name();
	LOG(VL_ERROR, "Actor::GetComponent: Actor does not have a component of type %s. Creating component...", t.c_str());
	return 0;
}


}

#endif